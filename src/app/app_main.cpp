// ak: headers
#include "../base/base_include.h"
#include "../os/os_include.h"
#include "../window_layer/window_layer_include.h"
#include "../render/render_include.h"
#include "../font/font.h"
#include "../draw/draw_include.h"
#include "../audio/audio.h"
#include "./app.hpp"
#include "./generated/app.meta.h"
#include "../game/game.hpp"
#include "../game/generated/game.meta.h"

// ak: implementation
#include "../base/base_include.c"
#include "../os/os_include.c"
#include "../window_layer/window_layer_include.c"
#include "../render/render_include.c"
#include "../font/font.c"
#include "../draw/draw_include.c"
#include "../audio/audio.c"
#include "./app.cpp"
#include "./generated/app.meta.c"
#include "../game/game.cpp"
#include "../game/generated/game.meta.c"

internal size_t app_get_u64_from_path(Str8 path, Arena *arena)
{
    uint64_t result = 0;
    if (path.length != 0 && fs_file_path_exists(path))
    {
        U8Array data = fs_file_path_read_full(path, arena);
        if (data.length != 0)
        {
            Str8 content = str8_skip_chop_whitespace(str8_init(data.v, data.length));
            uint64_t score_u64 = 0;
            if (try_u64_from_str8_c_rules(content, &score_u64))
            {
                result = score_u64;
            }
        }
    }
    return result;
}

internal void app_save_u64_in_path(Str8 path, uint64_t number, Arena *arena)
{
    if (path.length == 0)
    {
        return;
    }
    Str8 dir = str8_chop_last_slash(path);
    if (dir.length != 0 && !fs_is_dir_exist(dir))
    {
        fs_dir_make(dir);
    }
    Fs_File file = fs_file_open(path, Fs_File_Access_Flag_Write);
    if ((int32_t)file >= 0)
    {
        Str8 content = str8f(arena, "%zu\n", number);
        fs_file_write(file, content.cstr, (Rng1_U64){0, content.length});
        fs_file_close(file);
    }
}

internal void base_main(void)
{
    // ak: Application Init ===================================================
    wl_init();
    Wl_Window window = wl_window_open(APP_NAME);
    // wl_window_border_set(window, false);
    render_init();
    font_init();
    audio_init(48000, 2);
    Render_Handle window_equip = render_window_equip(window);
    game_init();
    Str8 data_home = os_get_data_home_path();
    Str8 score_path = str8f(game_state->arena, "%S/%S", data_home, APP_CMD_NAME);
    game_state->score.max = app_get_u64_from_path(score_path, game_state->arena);
    size_t last_saved_max_score = game_state->score.max;
    wm_window_set_fullscreen(window, true);
    
    // ak: Application Loop ===================================================
    while (!wl_should_exit())
    {
        font_frame();
        Arena_Temp scratch = arena_scratch_begin(0, 0);
        Wl_Event_List events = wl_get_events(scratch.arena, 0);
        wl_set_fps(15);
        
        render_begin_frame();
        render_window_begin_frame(window, window_equip);
        
        draw_begin_frame();
        Rng2_F32 canvas_rect = wl_canvas_rect_from_window(window);
        Draw_Bucket *bucket = draw_bucket_make();
        DrawBucketScope(bucket)
        {
            draw_rect(canvas_rect, game_state->color.background, 0.f, 0.f, 0.f);
            game_state->rect.canvas = canvas_rect;
            game_state->event.window_resize = false;
            for(Wl_Event *event = events.first; event != 0; event = event->next)
            {
                switch (event->kind)
                {
                    default: break;
                    case Wl_Event_Kind_WindowResize: {
                        game_state->event.window_resize = true;
                    } break;
                    case Wl_Event_Kind_WindowClose: {
                        wl_exit();
                    } break;
                    case Wl_Event_Kind_Press:
                    {
                        switch (event->key) {
                            default: break;
                            case Wl_Key_Q:
                            {
                                wl_exit();
                            } break;
                            case Wl_Key_Return:
                            {
                                if (game_state->game_over)
                                {
                                    game_state->game_over = false;
                                    game_state->score.current = 0;
                                }
                            } break;
                            case Wl_Key_Up:
                            {
                                if (game_state->event.direction != Game_Direction_Down)
                                {
                                    game_state->event.direction = Game_Direction_Up;
                                }
                            } break;
                            case Wl_Key_Down:
                            {
                                if (game_state->event.direction != Game_Direction_Up)
                                {
                                    game_state->event.direction = Game_Direction_Down;
                                }
                            } break;
                            case Wl_Key_Left:
                            {
                                if (game_state->event.direction != Game_Direction_Right)
                                {
                                    game_state->event.direction = Game_Direction_Left;
                                }
                            } break;
                            case Wl_Key_Right:
                            {
                                if (game_state->event.direction != Game_Direction_Left)
                                {
                                    game_state->event.direction = Game_Direction_Right;
                                }
                            } break;
                            case Wl_Key_F11:
                            {
                                bool is_fullscreen = wm_window_is_fullscreen(window);
                                wm_window_set_fullscreen(window, !is_fullscreen);
                            } break;
                        }
                    }
                    break;
                }
            }
            game_loop(scratch.arena);
            if (game_state->score.max > last_saved_max_score)
            {
                app_save_u64_in_path(score_path, game_state->score.max, scratch.arena);
                last_saved_max_score = game_state->score.max;
            }
        }
        draw_submit_bucket(window, window_equip, bucket);
        render_window_end_frame(window, window_equip);
        render_end_frame();
        arena_scratch_end(scratch);
    }
    
    // ak: Free Everything ====================================================
    render_window_unequip(window, window_equip);
    render_cleanup();
    audio_cleanup();
    wl_window_cleanup();
}
