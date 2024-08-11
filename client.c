#ifdef _WIN32
#include <SDL.h>
#undef main
#else
#include <SDL2/SDL.h>
#endif

#include <stdbool.h>
#include <stdio.h>

#include <gst/gst.h>
#include <gst/app/app.h>

struct client_context
{
    int width;
    int height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    GstElement *pipeline;
    GstElement *video_sink;
    bool active;
};

static struct client_context *client_context_new(int width, int height);

static void client_context_free(struct client_context *context);

static void client_context_loop(struct client_context *context);

static void client_context_resize(struct client_context *context, int width, int height);

int main(int argc, char *argv[])
{
    int result;
    SDL_DisplayMode display_mode;
    struct client_context *context;

    result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    if (result < 0)
    {
        puts(SDL_GetError());
        goto done;
    }

    result = SDL_GetCurrentDisplayMode(0, &display_mode);
    if (result < 0)
    {
        puts(SDL_GetError());
        goto done;
    }

    gst_init(NULL, NULL);

    context = client_context_new(display_mode.w / 2, display_mode.h / 2);
    if (context == NULL)
    {
        goto done;
    }

    client_context_loop(context);

done:
    client_context_free(context);
    return 0;
}

struct client_context *client_context_new(int width, int height)
{
    struct client_context *context;

    context = SDL_malloc(sizeof(struct client_context));

    context->width = width;
    context->height = height;

    context->window = SDL_CreateWindow("GStreamer SDL Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (context->window == NULL)
    {
        puts(SDL_GetError());
        goto error;
    }

    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED);
    if (context->renderer == NULL)
    {
        puts(SDL_GetError());
        goto error;
    }

    context->pipeline = gst_parse_launch("videotestsrc name=testsrc ! appsink name=video", NULL);
    context->video_sink = gst_bin_get_by_name(GST_BIN(context->pipeline), "video");

    client_context_resize(context, context->width, context->height);

    goto done;
error:
    client_context_free(context);
    context = NULL;
done:
    return context;
}

void client_context_free(struct client_context *context)
{
    if (context != NULL)
    {
        if (context->video_sink)
        {
            gst_object_unref(context->video_sink);
        }
        if (context->pipeline)
        {
            gst_object_unref(context->pipeline);
        }
        if (context->texture)
        {
            SDL_DestroyTexture(context->texture);
        }
        if (context->renderer)
        {
            SDL_DestroyRenderer(context->renderer);
        }
        if (context->window)
        {
            SDL_DestroyWindow(context->window);
        }

        SDL_free(context);
    }
}

void client_context_loop(struct client_context *context)
{
    SDL_Event event;
    GstSample *video_sample;
    GstBuffer *video_buffer;

    void *pixels;
    int pitch;

    gst_element_set_state(context->pipeline, GST_STATE_PLAYING);
    context->active = true;
    while (context->active)
    {
        if (SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                context->active = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                client_context_resize(context, (int)event.window.data1, (int)event.window.data2);
                break;
            }
        }
        video_sample = gst_app_sink_pull_sample(GST_APP_SINK(context->video_sink));
        if (video_sample)
        {
            video_buffer = gst_sample_get_buffer(video_sample);
            if (video_buffer)
            {
                SDL_LockTexture(context->texture, NULL, &pixels, &pitch);
                gst_buffer_extract(video_buffer, 0, pixels, pitch * context->height);
                SDL_UnlockTexture(context->texture);
            }
            gst_sample_unref(video_sample);
        }
        SDL_RenderCopy(context->renderer, context->texture, NULL, NULL);
        SDL_RenderPresent(context->renderer);
    }
}

void client_context_resize(struct client_context *context, int width, int height)
{
    GstCaps *caps;
    if (context->texture)
    {
        SDL_DestroyTexture(context->texture);
    }
    context->texture = SDL_CreateTexture(context->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, context->width, context->height);
    if (!context->texture)
    {
        puts(SDL_GetError());
    }

    caps = gst_caps_new_simple("video/x-raw",
                               "format", G_TYPE_STRING, "RGBA",
                               "width", G_TYPE_INT, context->width, "height", G_TYPE_INT, context->height,
                               "framerate", GST_TYPE_FRACTION, 30, 1, NULL);
    g_object_set(G_OBJECT(context->video_sink), "caps", caps, NULL);
    gst_element_set_state(context->pipeline, GST_STATE_PAUSED);
    gst_caps_unref(caps);
    context->width = width;
    context->height = height;
}