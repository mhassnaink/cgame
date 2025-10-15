/* cgame.h  -- single-header lightweight Windows "pygame-like" library
   Added: image loading/resizing/flipping/drawing using GDI+
   NOTE: link against GDI+ (gdiplus.lib). */

#ifndef CGAME_H
#define CGAME_H

#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)


    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <windowsx.h>
    #include <objidl.h>
    // Define PROPID if it's not defined
    #ifndef PROPID
    #define PROPID unsigned long
    #endif
    #include <gdiplus.h>
    #include <stdlib.h>
    #include <string.h>

    using namespace Gdiplus;

    // -------------------------
    // GDI+ helper globals
    // -------------------------
    static ULONG_PTR _cgame_gdiplusToken = 0;
    static bool _cgame_gdiplus_inited = false;

    // =========================
    // Events
    // =========================
    #define CGAME_QUIT         1
    #define CGAME_VIDEORESIZE  2
    #define CGAME_KEYDOWN      3
    #define CGAME_KEYUP        4

    // =========================
    // Display flags
    // =========================
    #define CGAME_RESIZABLE   0x01
    #define CGAME_DPI_AWARE   0x02
    #define CGAME_OPENGL      0x04
    #define CGAME_VULKAN      0x08
    #define CGAME_D3D12       0x10 

    // =========================
    // Console colors (Windows standard 16-color palette)
    // =========================
    #define CGAME_CONSOLE_COLOR_BLACK         0
    #define CGAME_CONSOLE_COLOR_BLUE          1
    #define CGAME_CONSOLE_COLOR_GREEN         2
    #define CGAME_CONSOLE_COLOR_CYAN          3
    #define CGAME_CONSOLE_COLOR_RED           4
    #define CGAME_CONSOLE_COLOR_MAGENTA       5
    #define CGAME_CONSOLE_COLOR_YELLOW        6
    #define CGAME_CONSOLE_COLOR_WHITE         7
    #define CGAME_CONSOLE_COLOR_GRAY          8
    #define CGAME_CONSOLE_COLOR_LIGHT_BLUE    9
    #define CGAME_CONSOLE_COLOR_LIGHT_GREEN   10
    #define CGAME_CONSOLE_COLOR_LIGHT_CYAN    11
    #define CGAME_CONSOLE_COLOR_LIGHT_RED     12
    #define CGAME_CONSOLE_COLOR_LIGHT_MAGENTA 13
    #define CGAME_CONSOLE_COLOR_LIGHT_YELLOW  14
    #define CGAME_CONSOLE_COLOR_BRIGHT_WHITE  15

    // =========================
    // Message box types
    // =========================
    #define CGAME_MB_OK             MB_OK
    #define CGAME_MB_OKCANCEL       MB_OKCANCEL
    #define CGAME_MB_YESNO          MB_YESNO
    #define CGAME_MB_ICONINFO       MB_ICONINFORMATION
    #define CGAME_MB_ICONWARNING    MB_ICONWARNING
    #define CGAME_MB_ICONERROR      MB_ICONERROR

    // =========================
    // Message box results
    // =========================
    #define CGAME_MB_IDOK      IDOK
    #define CGAME_MB_IDCANCEL  IDCANCEL
    #define CGAME_MB_IDYES     IDYES
    #define CGAME_MB_IDNO      IDNO

    // =========================
    // Screen struct
    // =========================
    typedef struct {
        HWND hwnd;
        HDC hdc;
        HGLRC hglrc;
        int width;
        int height;

        // API OPTIONS
        bool use_opengl;
        bool use_vulkan;
        bool use_d3d12;

        // Vulkan handles
        HMODULE vk_lib;
        void*   vk_instance;
        void*   vk_device;
        void*   vk_surface;
        void*   vk_swapchain;

        // Direct3D 12 handles
        HMODULE        d3d12_lib;
        void*          d3d12_device;
        void*          d3d12_cmdqueue;
        void*          d3d12_swapchain;
    } CGameScreen;

    // =========================
    // Ticks struct (timing)
    // =========================
    typedef struct {
        unsigned long ms; // milliseconds since game start
    } CGameTicks;

    // ========================= FPS MACRO
    #define CGAME_FPS_UNLIMITED 0




    // =========================
    // Key Constants
    // =========================
    enum {
        CGAME_K_UNKNOWN = 0,
        CGAME_K_a = 'A', CGAME_K_b = 'B', CGAME_K_c = 'C',
        CGAME_K_d = 'D', CGAME_K_e = 'E', CGAME_K_f = 'F',
        CGAME_K_g = 'G', CGAME_K_h = 'H', CGAME_K_i = 'I',
        CGAME_K_j = 'J', CGAME_K_k = 'K', CGAME_K_l = 'L',
        CGAME_K_m = 'M', CGAME_K_n = 'N', CGAME_K_o = 'O',
        CGAME_K_p = 'P', CGAME_K_q = 'Q', CGAME_K_r = 'R',
        CGAME_K_s = 'S', CGAME_K_t = 'T', CGAME_K_u = 'U',
        CGAME_K_v = 'V', CGAME_K_w = 'W', CGAME_K_x = 'X',
        CGAME_K_y = 'Y', CGAME_K_z = 'Z',

        CGAME_K_0 = '0', CGAME_K_1 = '1', CGAME_K_2 = '2',
        CGAME_K_3 = '3', CGAME_K_4 = '4', CGAME_K_5 = '5',
        CGAME_K_6 = '6', CGAME_K_7 = '7', CGAME_K_8 = '8',
        CGAME_K_9 = '9',

        CGAME_K_SPACE  = VK_SPACE,
        CGAME_K_RETURN = VK_RETURN,
        CGAME_K_ESCAPE = VK_ESCAPE,
        CGAME_K_LEFT   = VK_LEFT,
        CGAME_K_RIGHT  = VK_RIGHT,
        CGAME_K_UP     = VK_UP,
        CGAME_K_DOWN   = VK_DOWN,
    };

    // =========================
    // Mouse Constants
    // =========================
    #define CGameButtonLeft    1
    #define CGameButtonRight   2
    #define CGameButtonMiddle  3
    #define CGameButtonX1      4
    #define CGameButtonX2      5

    #define CGAME_MOUSEBUTTONDOWN  5
    #define CGAME_MOUSEBUTTONUP    6
    #define CGAME_MOUSEMOTION      7
    #define CGAME_MOUSEWHEEL       8


    // =========================
    // Internal globals
    // =========================
    static HINSTANCE   _cgame_hInstance = NULL;
    static bool        _cgame_running = true;
    static int         _cgame_event = 0;
    static CGameScreen _cgame_screen = { 0 };

    static COLORREF    _cgame_bgcolor = RGB(0,0,0);
    static HDC         _cgame_memdc = NULL;
    static HBITMAP     _cgame_membmp = NULL;
    static HGDIOBJ     _cgame_oldbmp = NULL;

    static WPARAM      _cgame_last_key = 0;
    static bool        _cgame_key_state[512] = { false };
    static bool        _cgame_key_prev_async[512] = { false };

    static bool        _cgame_mouse_state[5] = { false }; // left,right,middle,X1,X2
    static bool        _cgame_mouse_prev[5]  = { false };
    static int         _cgame_mouse_x = 0, _cgame_mouse_y = 0;
    static int         _cgame_mouse_wheel = 0;

    // font 
    static Gdiplus::PrivateFontCollection _cgame_font_collection;


    // Forward declarations
    static void _cgame_make_backbuffer(int w, int h);
    static void _cgame_free_backbuffer(void);
    /* OPENGL THE GOAT :) */
    static bool _cgame_init_opengl(HWND hwnd, HDC* hdc, HGLRC* hglrc);
    static void _cgame_cleanup_opengl(HDC hdc, HGLRC hglrc);

    /* Vulkan stubs / runtime detection (lightweight) */
    static bool _cgame_init_vulkan(HWND hwnd, HMODULE* vk_lib_out);
    static void _cgame_cleanup_vulkan(HMODULE vk_lib);

    /* DIRECT X 12 */
    static bool _cgame_init_d3d12(HWND hwnd, HMODULE* d3d12_lib_out);
    static void _cgame_cleanup_d3d12(HMODULE d3d12_lib);


    /* small helper to centralize FillRect usage */
    static void _cgame_fill_dc(HDC dc, int w, int h, COLORREF color);

    static WCHAR _cgame_window_title [256] = L"CGame Window";
    static HICON _cgame_window_icon = NULL;


    // =========================
    // Win32 window procedure
    // =========================
    static LRESULT CALLBACK _cgame_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_CLOSE:
            _cgame_event = CGAME_QUIT;
            _cgame_running = false;
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE: {
            int nw = LOWORD(lParam);
            int nh = HIWORD(lParam);

            // Only update if we have valid dimensions
            if (nw > 0 && nh > 0) {
                _cgame_screen.width = nw;
                _cgame_screen.height = nh;
                _cgame_event = CGAME_VIDEORESIZE;

                if (!_cgame_screen.use_opengl && !_cgame_screen.use_vulkan) {
                    _cgame_free_backbuffer();
                    _cgame_make_backbuffer(nw, nh);
                } else {
                    /* GPU-backed (OpenGL/Vulkan) — user should handle swapchain/viewport resizing */
                }
            }
            return 0;
        }

        case WM_PAINT: {
            if (!_cgame_screen.use_opengl && !_cgame_screen.use_vulkan && !_cgame_screen.use_d3d12) {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                if (_cgame_memdc && _cgame_membmp) {
                    BitBlt(hdc, 0, 0, _cgame_screen.width, _cgame_screen.height, _cgame_memdc, 0, 0, SRCCOPY);
                } else {
                    HBRUSH brush = CreateSolidBrush(_cgame_bgcolor);
                    FillRect(hdc, &ps.rcPaint, brush);
                    DeleteObject(brush);
                }
                EndPaint(hwnd, &ps);
            } else {
                /* For OpenGL/Vulkan/D3D12 we typically swap buffers / present from the GPU.
                If you want an automatic paint fallback for GPU modes, implement here. */
            }

        }

        case WM_KEYDOWN:
            _cgame_event = CGAME_KEYDOWN;
            _cgame_last_key = wParam;
            if ((wParam & 0xFF) < 512) _cgame_key_state[wParam & 0xFF] = true;
            return 0;

        case WM_KEYUP:
            _cgame_event = CGAME_KEYUP;
            _cgame_last_key = wParam;
            if ((wParam & 0xFF) < 512) _cgame_key_state[wParam & 0xFF] = false;
            return 0;

        case WM_LBUTTONDOWN:
            _cgame_event = CGAME_MOUSEBUTTONDOWN;
            _cgame_mouse_state[CGameButtonLeft] = true;
            return 0;

        case WM_LBUTTONUP:
            _cgame_event = CGAME_MOUSEBUTTONUP;
            _cgame_mouse_state[CGameButtonLeft] = false;
            return 0;

        case WM_RBUTTONDOWN:
            _cgame_event = CGAME_MOUSEBUTTONDOWN;
            _cgame_mouse_state[CGameButtonRight] = true;
            return 0;

        case WM_RBUTTONUP:
            _cgame_event = CGAME_MOUSEBUTTONUP;
            _cgame_mouse_state[CGameButtonRight] = false;
            return 0;

        case WM_MBUTTONDOWN:
            _cgame_event = CGAME_MOUSEBUTTONDOWN;
            _cgame_mouse_state[CGameButtonMiddle] = true;
            return 0;

        case WM_MBUTTONUP:
            _cgame_event = CGAME_MOUSEBUTTONUP;
            _cgame_mouse_state[CGameButtonMiddle] = false;
            return 0;

        case WM_MOUSEMOVE:
            _cgame_event = CGAME_MOUSEMOTION;
            _cgame_mouse_x = GET_X_LPARAM(lParam);
            _cgame_mouse_y = GET_Y_LPARAM(lParam);
            return 0;

        case WM_MOUSEWHEEL:
            _cgame_event = CGAME_MOUSEWHEEL;
            _cgame_mouse_wheel = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            return 0;

        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // =========================
    // Backbuffer helpers
    // =========================
    static void _cgame_fill_dc(HDC dc, int w, int h, COLORREF color) {
        if (!dc) return;
        HBRUSH brush = CreateSolidBrush(color);
        RECT rc = {0,0,w,h};
        FillRect(dc, &rc, brush);
        DeleteObject(brush);
    }

    static void _cgame_make_backbuffer(int w, int h) {
        if (!_cgame_screen.hwnd) return;
        HDC wnddc = GetDC(_cgame_screen.hwnd);
        if (!wnddc) return;

        _cgame_memdc = CreateCompatibleDC(wnddc);
        if (!_cgame_memdc) { ReleaseDC(_cgame_screen.hwnd, wnddc); return; }

        _cgame_membmp = CreateCompatibleBitmap(wnddc, w, h);
        _cgame_oldbmp = SelectObject(_cgame_memdc, _cgame_membmp);

        _cgame_fill_dc(_cgame_memdc, w, h, _cgame_bgcolor);

        ReleaseDC(_cgame_screen.hwnd, wnddc);
    }

    static void _cgame_free_backbuffer(void) {
        if (_cgame_memdc) {
            if (_cgame_oldbmp) {
                SelectObject(_cgame_memdc, _cgame_oldbmp);
                _cgame_oldbmp = NULL;
            }
            if (_cgame_membmp) {
                DeleteObject(_cgame_membmp);
                _cgame_membmp = NULL;
            }
            DeleteDC(_cgame_memdc);
            _cgame_memdc = NULL;
        }
    }

    // =========================
    // OpenGL helpers
    // =========================
    static bool _cgame_init_opengl(HWND hwnd, HDC* hdc, HGLRC* hglrc) {
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            24, 8, 0,
            PFD_MAIN_PLANE,
            0, 0, 0, 0
        };

        *hdc = GetDC(hwnd);
        if (!*hdc) return false;

        int pixelFormat = ChoosePixelFormat(*hdc, &pfd);
        if (!pixelFormat) {
            ReleaseDC(hwnd, *hdc);
            return false;
        }

        if (!SetPixelFormat(*hdc, pixelFormat, &pfd)) {
            ReleaseDC(hwnd, *hdc);
            return false;
        }

        *hglrc = wglCreateContext(*hdc);
        if (!*hglrc) {
            ReleaseDC(hwnd, *hdc);
            return false;
        }

        if (!wglMakeCurrent(*hdc, *hglrc)) {
            wglDeleteContext(*hglrc);
            ReleaseDC(hwnd, *hdc);
            return false;
        }

        return true;
    }
    

    static void _cgame_cleanup_opengl(HDC hdc, HGLRC hglrc) {
        if (hglrc) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
        }
        if (hdc) {
            ReleaseDC(_cgame_screen.hwnd, hdc);
        }
    }

    // =========================
    // Vulkan helpers (lightweight runtime detection)
    //  - This does not implement a full Vulkan setup. It attempts to
    //    load the Vulkan runtime (vulkan-1.dll) so the host has Vulkan
    //    available. Full instance/device/swapchain creation is left to
    //    the user or later extension code.
    // =========================
    static bool _cgame_init_vulkan(HWND hwnd, HMODULE* vk_lib_out) {
        if (!vk_lib_out) return false;
        *vk_lib_out = LoadLibraryA("vulkan-1.dll");
        if (!*vk_lib_out) {
            MessageBoxW(hwnd,
                        L"Vulkan runtime (vulkan-1.dll) not found.\nInstall Vulkan runtime/SDK or remove CGAME_VULKAN flag.",
                        L"CGame Vulkan Error",
                        MB_OK | MB_ICONERROR);
            return false;
        }
        /* We don't create a Vulkan instance here — this is a safe runtime detection
           scaffold so user code (or a later extension) can link Vulkan functions
           and create instance/device/swapchain as desired. */
        return true;
    }

    static void _cgame_cleanup_vulkan(HMODULE vk_lib) {
        if (vk_lib) {
            FreeLibrary(vk_lib);
        }
    }

    static bool _cgame_init_d3d12(HWND hwnd, HMODULE* d3d12_lib_out) {
        *d3d12_lib_out = LoadLibraryA("d3d12.dll");
        if (!*d3d12_lib_out) {
            MessageBoxW(hwnd,
                L"Direct3D 12 runtime (d3d12.dll) not found.\nInstall latest DirectX runtime or remove CGAME_D3D12 flag.",
                L"CGame D3D12 Error",
                MB_OK | MB_ICONERROR);
            return false;
        }

        /* Full D3D12 setup (device, swapchain, command queue) left to user.
        We only load the runtime dynamically and provide placeholders. */
        return true;
    }

    static void _cgame_cleanup_d3d12(HMODULE d3d12_lib) {
        if (d3d12_lib) FreeLibrary(d3d12_lib);
    }

    // =========================================================
    // Image subsystem
    // =========================================================
    typedef struct {
        int width;
        int height;
        int channels;    // usually 4 (RGBA)
        unsigned char* pixels; // raw pixel data (row-major, RGBA, 8-bit)
        HBITMAP hbitmap; // GDI HBITMAP for fast drawing/AlphaBlend where appropriate
    } CGameImage;

    // Helpers: free pixel + HBITMAP
    static void _cgame_image_free_internal(CGameImage* img) {
        if (!img) return;
        if (img->hbitmap) {
            DeleteObject(img->hbitmap);
            img->hbitmap = NULL;
        }
        if (img->pixels) {
            free(img->pixels);
            img->pixels = NULL;
        }
        img->width = img->height = img->channels = 0;
    }

    static HBITMAP _cgame_image_create_hbitmap_from_rgba(const CGameImage* img);
    static bool   _cgame_image_from_gdiplus_bitmap(Bitmap* bmp, CGameImage* out);


    // Fill fully-transparent pixels' RGB by copying the nearest opaque neighbor's RGB.
    // max_radius: search radius in pixels (3..8 is reasonable).
    static void _cgame_image_clean_edges(CGameImage *img, int max_radius) {
        if (!img || !img->pixels || img->channels < 4) return;
        int w = img->width, h = img->height, c = img->channels;
        size_t total = (size_t)w * h * c;
        unsigned char *src = (unsigned char*)malloc(total);
        if (!src) return;
        memcpy(src, img->pixels, total);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                size_t idx = ((size_t)y * w + x) * c;
                unsigned char a = src[idx + 3];
                if (a != 0) continue; // only fix fully transparent

                bool found = false;
                for (int r = 1; r <= max_radius && !found; ++r) {
                    int xmin = x - r, xmax = x + r;
                    int ymin = y - r, ymax = y + r;
                    if (xmin < 0) xmin = 0;
                    if (ymin < 0) ymin = 0;
                    if (xmax >= w) xmax = w - 1;
                    if (ymax >= h) ymax = h - 1;

                    for (int yy = ymin; yy <= ymax && !found; ++yy) {
                        for (int xx = xmin; xx <= xmax; ++xx) {
                            // perimeter optimization: only check ring
                            if (abs(xx - x) != r && abs(yy - y) != r) continue;
                            size_t j = ((size_t)yy * w + xx) * c;
                            if (src[j + 3] != 0) {
                                img->pixels[idx + 0] = src[j + 0];
                                img->pixels[idx + 1] = src[j + 1];
                                img->pixels[idx + 2] = src[j + 2];
                                // keep alpha = 0
                                img->pixels[idx + 3] = 0;
                                found = true;
                                break;
                            }
                        }
                    }
                }
                // if not found, leave RGB as-is
            }
        }

        free(src);
    }

    // Load image from file path using GDI+
    static CGameImage _cgame_image_load_impl(const char* path) {
        CGameImage img = {0,0,0,NULL,NULL};
        if (!path) return img;

        // Ensure GDI+ started
        if (!_cgame_gdiplus_inited) {
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            if (GdiplusStartup(&_cgame_gdiplusToken, &gdiplusStartupInput, NULL) == Gdiplus::Ok) {
                _cgame_gdiplus_inited = true;
            } else {
                return img;
            }
        }

        // Convert path to wide char
        int needed = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
        if (needed <= 0) return img;
        WCHAR* wpath = (WCHAR*)malloc(needed * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, needed);

        Bitmap* bmp = Bitmap::FromFile(wpath, FALSE);
        free(wpath);
        if (!bmp) return img;
        if (bmp->GetLastStatus() != Gdiplus::Ok) {
            delete bmp;
            return img;
        }

        if (!_cgame_image_from_gdiplus_bitmap(bmp, &img)) {
            delete bmp;
            return img;
        }

        delete bmp;
        return img;
    }

    // Free image
    static void _cgame_image_unload_impl(CGameImage* img) {
        if (!img) return;
        _cgame_image_free_internal(img);
    }

    // Create nearest-neighbour resized copy
    static CGameImage _cgame_image_resize_nearest(const CGameImage* src, int new_w, int new_h) {
        CGameImage out = {0,0,0,NULL,NULL};
        if (!src || src->width <=0 || src->height <=0 || new_w<=0 || new_h<=0) return out;

        out.width = new_w;
        out.height = new_h;
        out.channels = src->channels;
        size_t total = (size_t)new_w * (size_t)new_h * out.channels;
        out.pixels = (unsigned char*)malloc(total);
        if (!out.pixels) { out.width = out.height = out.channels = 0; return out; }

        for (int y = 0; y < new_h; ++y) {
            int sy = (int)(((long long)y * src->height) / new_h);
            if (sy >= src->height) sy = src->height - 1;

            for (int x = 0; x < new_w; ++x) {
                int sx = (int)(((long long)x * src->width) / new_w);
                if (sx >= src->width) sx = src->width - 1;

                unsigned char* sp = src->pixels + ((size_t)sy * src->width + sx) * src->channels;
                unsigned char* dp = out.pixels + ((size_t)y * new_w + x) * out.channels;

                if (src->channels == 4) {
                    // RGBA → copy all 4 channels (keeps alpha intact)
                    dp[0] = sp[0]; // R
                    dp[1] = sp[1]; // G
                    dp[2] = sp[2]; // B
                    dp[3] = sp[3]; // A (preserves transparency)
                } else {
                    // RGB → copy as usual
                    for (int c = 0; c < out.channels; ++c) {
                        dp[c] = sp[c];
                    }
                }
            }
        }

        // After out.pixels created above:
        // Clean edges a bit after resize to avoid introducing halos
        if (out.channels == 4) {
            _cgame_image_clean_edges(&out, 3); // smaller radius after resize
            // create HBITMAP via helper (handles premultiply & stride)
            HBITMAP hb = _cgame_image_create_hbitmap_from_rgba(&out);
            if (hb) {
                out.hbitmap = hb;
            } else {
                out.hbitmap = NULL;
            }
        }


        return out;
    }

    // Flip horizontal (in-place)
    static bool _cgame_image_flip_horizontal_impl(CGameImage* img) {
        if (!img || !img->pixels) return false;
        int w = img->width, h = img->height, c = img->channels;
        int row_bytes = w * c;
        unsigned char* tmp = (unsigned char*)malloc(row_bytes);
        if (!tmp) return false;

        for (int y=0;y<h;++y) {
            unsigned char* row = img->pixels + (size_t)y * row_bytes;
            for (int x=0;x<w/2;++x) {
                unsigned char* a = row + x*c;
                unsigned char* b = row + (w-1-x)*c;
                for (int k=0;k<c;++k) {
                    unsigned char t = a[k];
                    a[k] = b[k];
                    b[k] = t;
                }
            }
        }
        free(tmp);

        // Also recreate HBITMAP if present
        if (img->hbitmap) {
            DeleteObject(img->hbitmap);
            img->hbitmap = NULL;
            // create new HBITMAP from pixels
            BITMAPINFO bmi;
            memset(&bmi,0,sizeof(bmi));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h; // top-down
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            // recreate HBITMAP from current (straight) RGBA pixels
            HBITMAP hb = _cgame_image_create_hbitmap_from_rgba(img);
            if (hb) {
                img->hbitmap = hb;
            } else {
                img->hbitmap = NULL;
            }
        }

        return true;
    }

    // Flip vertical (in-place)
    static bool _cgame_image_flip_vertical_impl(CGameImage* img) {
        if (!img || !img->pixels) return false;
        int w = img->width, h = img->height, c = img->channels;
        int row_bytes = w * c;
        unsigned char* tmp = (unsigned char*)malloc(row_bytes);
        if (!tmp) return false;

        for (int y=0;y<h/2;++y) {
            unsigned char* a = img->pixels + (size_t)y * row_bytes;
            unsigned char* b = img->pixels + (size_t)(h-1-y) * row_bytes;
            memcpy(tmp, a, row_bytes);
            memcpy(a, b, row_bytes);
            memcpy(b, tmp, row_bytes);
        }
        free(tmp);

        // Also recreate HBITMAP if present (same as horizontal)
        if (img->hbitmap) {
            DeleteObject(img->hbitmap);
            img->hbitmap = NULL;
            BITMAPINFO bmi;
            memset(&bmi,0,sizeof(bmi));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h; // top-down
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            // recreate HBITMAP from current (straight) RGBA pixels
            HBITMAP hb = _cgame_image_create_hbitmap_from_rgba(img);
            if (hb) {
                img->hbitmap = hb;
            } else {
                img->hbitmap = NULL;
            }

        }

        return true;
    }

    // Draw image at (x,y). If image has HBITMAP, use AlphaBlend with compatible DC.
    static void _cgame_image_draw_impl(const CGameImage* img, int x, int y) {
        if (!img || !img->pixels) return;
        if (!_cgame_memdc || _cgame_screen.use_opengl || _cgame_screen.use_vulkan) return;

        int w = img->width, h = img->height;
        if (img->hbitmap) {
            // create memory DC for source
            HDC srcdc = CreateCompatibleDC(_cgame_memdc);
            HGDIOBJ old = SelectObject(srcdc, img->hbitmap);

            BLENDFUNCTION bf;
            bf.BlendOp = AC_SRC_OVER;
            bf.BlendFlags = 0;
            bf.SourceConstantAlpha = 255;
            bf.AlphaFormat = AC_SRC_ALPHA;

            AlphaBlend(_cgame_memdc, x, y, w, h, srcdc, 0, 0, w, h, bf);

            SelectObject(srcdc, old);
            DeleteDC(srcdc);
        } else {
            // fallback: create DIB section from pixel buffer, draw then delete
            BITMAPINFO bmi;
            memset(&bmi,0,sizeof(bmi));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            void* bits = NULL;
            HBITMAP hb = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
            if (!hb) return;

            // stride (DWORD aligned)
            int stride = ((w * 32 + 31) / 32) * 4;
            unsigned char* dstAll = (unsigned char*)bits;
            const unsigned char* srcAll = img->pixels;
            int c = img->channels;
            for (int yy = 0; yy < h; ++yy) {
                unsigned char* dstRow = dstAll + (size_t)yy * stride;
                const unsigned char* srcRow = srcAll + (size_t)yy * w * c;
                for (int xx = 0; xx < w; ++xx) {
                    unsigned char r = srcRow[xx * c + 0];
                    unsigned char g = srcRow[xx * c + 1];
                    unsigned char b = srcRow[xx * c + 2];
                    unsigned char a = (c >= 4) ? srcRow[xx * c + 3] : 255;

                    dstRow[xx*4 + 0] = (unsigned char)((unsigned int)b * (unsigned int)a / 255u);
                    dstRow[xx*4 + 1] = (unsigned char)((unsigned int)g * (unsigned int)a / 255u);
                    dstRow[xx*4 + 2] = (unsigned char)((unsigned int)r * (unsigned int)a / 255u);
                    dstRow[xx*4 + 3] = a;
                }
            }


            HDC srcdc = CreateCompatibleDC(_cgame_memdc);
            HGDIOBJ old = SelectObject(srcdc, hb);

            BLENDFUNCTION bf;
            bf.BlendOp = AC_SRC_OVER;
            bf.BlendFlags = 0;
            bf.SourceConstantAlpha = 255;
            bf.AlphaFormat = AC_SRC_ALPHA;

            AlphaBlend(_cgame_memdc, x, y, w, h, srcdc, 0, 0, w, h, bf);

            SelectObject(srcdc, old);
            DeleteDC(srcdc);
            DeleteObject(hb);
        }
    }

    // Create a top-down DIB HBITMAP from RGBA (straight) image pixels.
    // This function performs premultiplication (RGBA -> premultiplied BGRA)
    // and respects the DIB stride (DWORD aligned).
    static HBITMAP _cgame_image_create_hbitmap_from_rgba(const CGameImage* img) {
        if (!img || !img->pixels) return NULL;
        int w = img->width, h = img->height;
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* bits = NULL;
        HBITMAP hb = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
        if (!hb || !bits) {
            if (hb) DeleteObject(hb);
            return NULL;
        }

        // stride (bytes per row), DWORD aligned
        int stride = ((w * 32 + 31) / 32) * 4;
        unsigned char* destRow = (unsigned char*)bits;
        const unsigned char* src = img->pixels;
        int c = img->channels;

        for (int y = 0; y < h; ++y) {
            unsigned char* dst = destRow + (size_t)y * stride;
            const unsigned char* srow = src + (size_t)y * w * c;
            for (int x = 0; x < w; ++x) {
                unsigned char r = srow[x * c + 0];
                unsigned char g = srow[x * c + 1];
                unsigned char b = srow[x * c + 2];
                unsigned char a = (c >= 4) ? srow[x * c + 3] : 255;

                // premultiply -> store BGRA
                dst[x*4 + 0] = (unsigned char)((unsigned int)b * (unsigned int)a / 255u);
                dst[x*4 + 1] = (unsigned char)((unsigned int)g * (unsigned int)a / 255u);
                dst[x*4 + 2] = (unsigned char)((unsigned int)r * (unsigned int)a / 255u);
                dst[x*4 + 3] = a;
            }
        }

        return hb;
    }

    static bool _cgame_image_from_gdiplus_bitmap(Bitmap* bmp, CGameImage* out) {
        if (!bmp || !out) return false;
        UINT w = bmp->GetWidth();
        UINT h = bmp->GetHeight();

        BitmapData bd;
        Rect r(0,0,w,h);

        // Request 32bpp ARGB (premultiplied) from GDI+
        if (bmp->LockBits(&r, ImageLockModeRead, PixelFormat32bppARGB, &bd) != Gdiplus::Ok) {
            return false;
        }

        size_t stride = (size_t)w * 4;
        unsigned char* pixels = (unsigned char*)malloc((size_t)w * (size_t)h * 4);
        if (!pixels) {
            bmp->UnlockBits(&bd);
            return false;
        }

        unsigned char* src = (unsigned char*)bd.Scan0;
        for (UINT y = 0; y < h; ++y) {
            unsigned char* srow = src + (size_t)y * bd.Stride;
            unsigned char* dst = pixels + (size_t)y * stride;
            for (UINT x = 0; x < w; ++x) {
                unsigned char b_p = srow[x*4 + 0];
                unsigned char g_p = srow[x*4 + 1];
                unsigned char r_p = srow[x*4 + 2];
                unsigned char a   = srow[x*4 + 3];

                // Convert premultiplied BGRA -> straight RGBA
                if (a == 0) {
                    dst[x*4 + 0] = 0;
                    dst[x*4 + 1] = 0;
                    dst[x*4 + 2] = 0;
                    dst[x*4 + 3] = 0;
                } else {
                    // un-premultiply: r = r_p * 255 / a (integer-safe)
                    dst[x*4 + 0] = (unsigned char)((unsigned int)r_p * 255u / (unsigned int)a);
                    dst[x*4 + 1] = (unsigned char)((unsigned int)g_p * 255u / (unsigned int)a);
                    dst[x*4 + 2] = (unsigned char)((unsigned int)b_p * 255u / (unsigned int)a);
                    dst[x*4 + 3] = a;
                }
            }
        }

        bmp->UnlockBits(&bd);

        // Fill out struct
        out->width = (int)w;
        out->height = (int)h;
        out->channels = 4;
        out->pixels = pixels;
        out->hbitmap = NULL;

        // Clean edges to avoid halo (use modest radius)
        _cgame_image_clean_edges(out, 5);

        // Create HBITMAP (premultiplied BGRA) for faster drawing
        HBITMAP hbm = _cgame_image_create_hbitmap_from_rgba(out);
        if (hbm) out->hbitmap = hbm;

        return true;
    }



    // Draw scaled (new_w,new_h)
    static void _cgame_image_draw_scaled_impl(const CGameImage* src, int x, int y, int new_w, int new_h) {
        if (!src || !src->pixels) return;
        // create nearest-resized temp and draw
        CGameImage tmp = _cgame_image_resize_nearest(src, new_w, new_h);
        _cgame_image_draw_impl(&tmp, x, y);
        _cgame_image_unload_impl(&tmp);
    }

    // Getters
    static int _cgame_image_get_width_impl(const CGameImage* img) { return img ? img->width : 0; }
    static int _cgame_image_get_height_impl(const CGameImage* img) { return img ? img->height : 0; }
    static int _cgame_image_get_channels_impl(const CGameImage* img) { return img ? img->channels : 0; }

    static HICON _cgame_create_icon_from_image(const CGameImage* img) {
        if (!img || !img->pixels) return NULL;

        // Prepare BITMAPV5HEADER for 32-bit ARGB
        BITMAPV5HEADER bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.bV5Size        = sizeof(BITMAPV5HEADER);
        bi.bV5Width       = img->width;
        bi.bV5Height      = -img->height; // top-down
        bi.bV5Planes      = 1;
        bi.bV5BitCount    = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask     =  0x00FF0000;
        bi.bV5GreenMask   =  0x0000FF00;
        bi.bV5BlueMask    =  0x000000FF;
        bi.bV5AlphaMask   =  0xFF000000;

        HDC hdc = GetDC(NULL);
        void* bits = NULL;
        HBITMAP colorBmp = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bits, NULL, 0);
        ReleaseDC(NULL, hdc);
        if (!colorBmp || !bits) {
            if (colorBmp) DeleteObject(colorBmp);
            return NULL;
        }

        // Copy pixels (straight RGBA → BGRA with alpha)
        int w = img->width, h = img->height, c = img->channels;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                unsigned char* dst = (unsigned char*)bits + (y * w + x) * 4;
                const unsigned char* src = img->pixels + ((y * w + x) * c);

                dst[0] = src[2]; // B
                dst[1] = src[1]; // G
                dst[2] = src[0]; // R
                dst[3] = (c >= 4) ? src[3] : 255; // A
            }
        }

        // Create monochrome mask (unused but required)
        HBITMAP maskBmp = CreateBitmap(w, h, 1, 1, NULL);

        ICONINFO ii;
        ZeroMemory(&ii, sizeof(ii));
        ii.fIcon    = TRUE;
        ii.xHotspot = 0;
        ii.yHotspot = 0;
        ii.hbmMask  = maskBmp;
        ii.hbmColor = colorBmp;

        HICON hIcon = CreateIconIndirect(&ii);

        DeleteObject(colorBmp);
        DeleteObject(maskBmp);

        return hIcon;
    }

    static bool _cgame_display_set_icon_from_image(const CGameImage* img) {
        if (!img) return false;
        HICON hIcon = _cgame_create_icon_from_image(img);
        if (!hIcon) return false;

        _cgame_window_icon = hIcon;
        if (_cgame_screen.hwnd) {
            SendMessage(_cgame_screen.hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            SendMessage(_cgame_screen.hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        return true;
    }



    // =========================
    // Core functions
    // =========================
    static void _cgame_init(void) {
        _cgame_hInstance = GetModuleHandleW(NULL);
        _cgame_running = true;
        _cgame_event = 0;

        // Start GDI+ (if not already)
        if (!_cgame_gdiplus_inited) {
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            if (GdiplusStartup(&_cgame_gdiplusToken, &gdiplusStartupInput, NULL) == Gdiplus::Ok) {
                _cgame_gdiplus_inited = true;
            }
        }
    }
    
    // terminal color
    static void _cgame_console_set_color(int fg, int bg) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD)((bg << 4) | (fg & 0x0F)));
    }

    static void _cgame_console_reset_color(void) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD)(CGAME_CONSOLE_COLOR_WHITE | (CGAME_CONSOLE_COLOR_BLACK << 4)));
    }


    // message box wrapper
    static int _cgame_message_box(const wchar_t* title, const wchar_t* text, UINT type) {
        return MessageBoxW(NULL, text, title, type);
    }


    static void _cgame_quit(void) {
        _cgame_running = false;
        if (_cgame_screen.hwnd) {
            if (_cgame_screen.use_opengl) {
                _cgame_cleanup_opengl(_cgame_screen.hdc, _cgame_screen.hglrc);
            } else if (_cgame_screen.use_vulkan) {
                _cgame_cleanup_vulkan(_cgame_screen.vk_lib);
            } else if (_cgame_screen.use_d3d12) {
                _cgame_cleanup_d3d12(_cgame_screen.d3d12_lib);
            } else {
                if (_cgame_screen.hdc) ReleaseDC(_cgame_screen.hwnd, _cgame_screen.hdc);
            }
            DestroyWindow(_cgame_screen.hwnd);
        }
        _cgame_free_backbuffer();
        UnregisterClassW(L"CGameWindowClass", _cgame_hInstance);

        // Shutdown GDI+ if inited
        if (_cgame_gdiplus_inited) {
            GdiplusShutdown(_cgame_gdiplusToken);
            _cgame_gdiplus_inited = false;
            _cgame_gdiplusToken = 0;
        }
    }

    // =========================
    // Display functions
    // =========================
    static CGameScreen _cgame_set_mode_impl(int w, int h, int flags) {
        // --- Check conflicting GPU API flags ---
        int gpu_api_count = (!!(flags & CGAME_OPENGL)) +
                            (!!(flags & CGAME_VULKAN)) +
                            (!!(flags & CGAME_D3D12));
        if (gpu_api_count > 1) {
            MessageBoxW(NULL,
                L"Cannot combine multiple GPU APIs.\nChoose only one: OpenGL, Vulkan, or Direct3D 12.",
                L"CGame Error",
                MB_OK | MB_ICONERROR);
            return _cgame_screen;
        }

        if (flags & CGAME_DPI_AWARE) SetProcessDPIAware();

        // --- Register window class ---
        WNDCLASSW wc = {0};
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = _cgame_WndProc;
        wc.hInstance     = _cgame_hInstance;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = L"CGameWindowClass";
        RegisterClassW(&wc);

        // --- Window style ---
        DWORD style = WS_OVERLAPPEDWINDOW;
        if (!(flags & CGAME_RESIZABLE)) {
            style &= ~WS_THICKFRAME;
            style &= ~WS_MAXIMIZEBOX;
        }

        // --- Adjust rect for client size ---
        RECT rect = {0, 0, w, h};
        AdjustWindowRect(&rect, style, FALSE);
        int adjWidth  = rect.right - rect.left;
        int adjHeight = rect.bottom - rect.top;

        // --- Create window ---
        HWND hwnd = CreateWindowExW(
            0,
            wc.lpszClassName,
            (_cgame_window_title[0] != L'\0') ? _cgame_window_title : L"CGame Window",
            style,
            CW_USEDEFAULT, CW_USEDEFAULT, adjWidth, adjHeight,
            NULL, NULL, _cgame_hInstance, NULL
        );


        if (!hwnd) {
            MessageBoxW(NULL, L"Failed to create window.", L"CGame Error", MB_OK | MB_ICONERROR);
            return _cgame_screen;
        }

        _cgame_screen.hwnd = hwnd;

        // --- Apply stored title (if set after creation, it still updates) ---
        if (_cgame_window_title[0]) {
            SetWindowTextW(hwnd, _cgame_window_title);
        }

        // --- Apply stored icon (if any) ---
        if (_cgame_window_icon) {
            SendMessage(hwnd, WM_SETICON, ICON_BIG,   (LPARAM)_cgame_window_icon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)_cgame_window_icon);
        }

        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        // --- Fill screen struct ---
        _cgame_screen.use_opengl = (flags & CGAME_OPENGL) != 0;
        _cgame_screen.use_vulkan = (flags & CGAME_VULKAN) != 0;
        _cgame_screen.use_d3d12  = (flags & CGAME_D3D12) != 0;

        _cgame_screen.vk_lib      = NULL;
        _cgame_screen.vk_instance = NULL;
        _cgame_screen.vk_device   = NULL;
        _cgame_screen.vk_surface  = NULL;
        _cgame_screen.vk_swapchain= NULL;

        _cgame_screen.d3d12_lib   = NULL;
        _cgame_screen.d3d12_device    = NULL;
        _cgame_screen.d3d12_cmdqueue  = NULL;
        _cgame_screen.d3d12_swapchain = NULL;

        // --- Get actual client area size ---
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        _cgame_screen.width  = clientRect.right - clientRect.left;
        _cgame_screen.height = clientRect.bottom - clientRect.top;

        // --- Choose API ---
        if (_cgame_screen.use_opengl) {
            if (!_cgame_init_opengl(hwnd, &_cgame_screen.hdc, &_cgame_screen.hglrc)) {
                DestroyWindow(hwnd);
                _cgame_screen.hwnd = NULL;
            }
        } else if (_cgame_screen.use_vulkan) {
            if (!_cgame_init_vulkan(hwnd, &_cgame_screen.vk_lib)) {
                DestroyWindow(hwnd);
                _cgame_screen.hwnd = NULL;
            } else {
                _cgame_screen.hdc = NULL; // Vulkan doesn’t use GDI
            }
        } else if (_cgame_screen.use_d3d12) {
            if (!_cgame_init_d3d12(hwnd, &_cgame_screen.d3d12_lib)) {
                DestroyWindow(hwnd);
                _cgame_screen.hwnd = NULL;
            } else {
                _cgame_screen.hdc = NULL; // D3D12 doesn’t use GDI
            }
        } else {
            // CPU/GDI backbuffer
            _cgame_screen.hdc = GetDC(hwnd);
            _cgame_make_backbuffer(_cgame_screen.width, _cgame_screen.height);
        }

        return _cgame_screen;
    }



    static void _cgame_display_set_bgcolor(int r, int g, int b) {
        _cgame_bgcolor = RGB(r,g,b);
        if (!_cgame_screen.use_opengl && !_cgame_screen.use_vulkan && _cgame_memdc) {
            _cgame_fill_dc(_cgame_memdc, _cgame_screen.width, _cgame_screen.height, _cgame_bgcolor);
        }
        /* For OpenGL/Vulkan the bg color is stored in _cgame_bgcolor,
           but clearing should be done by the user's rendering code (glClear / vkCmdClearAttachments). */
    }

    static void _cgame_display_clear_impl(void) {
        if (!_cgame_screen.use_opengl && !_cgame_screen.use_vulkan && _cgame_memdc) {
            _cgame_fill_dc(_cgame_memdc, _cgame_screen.width, _cgame_screen.height, _cgame_bgcolor);
        }
        /* For OpenGL/Vulkan the user should call their own clear/present */
    }

    static void _cgame_display_flip_impl(void) {
        if (!_cgame_screen.hwnd) return;

        if (_cgame_screen.use_opengl) {
            SwapBuffers(_cgame_screen.hdc);
        } else if (_cgame_screen.use_vulkan) {
            /* For Vulkan mode, the user must call vkQueuePresentKHR / present their swapchain.
               This library only performs runtime detection and provides placeholders. */
        } else {
            HDC wnddc = GetDC(_cgame_screen.hwnd);
            if (wnddc && _cgame_memdc) {
                BitBlt(wnddc, 0, 0, _cgame_screen.width, _cgame_screen.height, _cgame_memdc, 0, 0, SRCCOPY);
            }
            if (wnddc) ReleaseDC(_cgame_screen.hwnd, wnddc);
        }
    }

    static int _cgame_display_get_width(void) { return _cgame_screen.width; }
    static int _cgame_display_get_height(void) { return _cgame_screen.height; }

    // =========================
    // Drawing
    // =========================
    static void _cgame_draw_fillrect_impl(int x, int y, int w, int h, int r, int g, int b) {
        if (!_cgame_memdc || _cgame_screen.use_opengl || _cgame_screen.use_vulkan) return;
        HBRUSH brush = CreateSolidBrush(RGB(r,g,b));
        RECT rc = { x, y, x + w, y + h };
        FillRect(_cgame_memdc, &rc, brush);
        DeleteObject(brush);
    }

    static void _cgame_draw_rect_impl(int x, int y, int w, int h, int r, int g, int b) {
        if (!_cgame_memdc || _cgame_screen.use_opengl || _cgame_screen.use_vulkan) return;
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(r,g,b));
        HGDIOBJ oldPen = SelectObject(_cgame_memdc, pen);
        HGDIOBJ oldBrush = SelectObject(_cgame_memdc, GetStockObject(NULL_BRUSH));

        Rectangle(_cgame_memdc, x, y, x + w, y + h);

        SelectObject(_cgame_memdc, oldPen);
        SelectObject(_cgame_memdc, oldBrush);
        DeleteObject(pen);
    }

    // =========================
    // Polygon drawing
    // =========================
    static void _cgame_draw_polygon_impl(const int* xs, const int* ys, int count,
                                         int r, int g, int b) {
        if (!_cgame_memdc || !xs || !ys || count < 2) return;
        Graphics gdi(_cgame_memdc);
        Pen pen(Color(255, r, g, b), 1.0f);

        Point* pts = (Point*)malloc(sizeof(Point) * count);
        for (int i = 0; i < count; i++) pts[i] = Point(xs[i], ys[i]);

        gdi.DrawPolygon(&pen, pts, count);
        free(pts);
    }

    static void _cgame_draw_fill_polygon_impl(const int* xs, const int* ys, int count,
                                              int r, int g, int b) {
        if (!_cgame_memdc || !xs || !ys || count < 3) return;
        Graphics gdi(_cgame_memdc);
        SolidBrush brush(Color(255, r, g, b));

        Point* pts = (Point*)malloc(sizeof(Point) * count);
        for (int i = 0; i < count; i++) pts[i] = Point(xs[i], ys[i]);

        gdi.FillPolygon(&brush, pts, count);
        free(pts);
    }

    // =========================
    // Rounded rectangle drawing
    // =========================
    static GraphicsPath* _cgame_create_rounded_rect_path(int x, int y, int w, int h, int radius) {
        GraphicsPath* path = new GraphicsPath();
        int diameter = radius * 2;
        if (diameter > w) diameter = w;
        if (diameter > h) diameter = h;

        path->AddArc(x, y, diameter, diameter, 180, 90);
        path->AddArc(x + w - diameter, y, diameter, diameter, 270, 90);
        path->AddArc(x + w - diameter, y + h - diameter, diameter, diameter, 0, 90);
        path->AddArc(x, y + h - diameter, diameter, diameter, 90, 90);
        path->CloseFigure();
        return path;
    }

    static void _cgame_draw_rounded_rect_impl(int x, int y, int w, int h, int radius,
                                              int border_width, int r, int g, int b) {
        if (!_cgame_memdc) return;
        Graphics gdi(_cgame_memdc);
        Pen pen(Color(255, r, g, b), (REAL)border_width);
        GraphicsPath* path = _cgame_create_rounded_rect_path(x, y, w, h, radius);
        gdi.DrawPath(&pen, path);
        delete path;
    }

    static void _cgame_draw_rounded_fill_rect_impl(int x, int y, int w, int h, int radius,
                                                   int r, int g, int b) {
        if (!_cgame_memdc) return;
        Graphics gdi(_cgame_memdc);
        SolidBrush brush(Color(255, r, g, b));
        GraphicsPath* path = _cgame_create_rounded_rect_path(x, y, w, h, radius);
        gdi.FillPath(&brush, path);
        delete path;
    }


    // =========================
    // Events
    // =========================
    static int _cgame_event_get(void) {
        MSG msg;
        // Keep checking until we find an event we care about
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                _cgame_running = false;
                return CGAME_QUIT;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);

            if (_cgame_event != 0) {
                int ev = _cgame_event;
                _cgame_event = 0; // clear AFTER we grab it
                return ev;
            }
        }
        return 0; // no event
    }

    // =========================
    // Text rendering
    // =========================
    static void _cgame_text_draw_impl(const char* text, int x, int y, int r, int g, int b) {
        if (!_cgame_memdc || !text) return;

        Graphics gdi(_cgame_memdc);
        SolidBrush brush(Color(255, r, g, b));
        Font font(L"Arial", 16); // default font, size 16
        WCHAR wtext[512];
        MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, 512);
        gdi.DrawString(wtext, -1, &font, PointF((REAL)x, (REAL)y), &brush);
    }

    // Load custom font (TTF file)
    static bool _cgame_text_load_font_impl(const char* path) {
        if (!path) return false;
        int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
        WCHAR* wpath = (WCHAR*)malloc(wlen * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

        if (_cgame_font_collection.AddFontFile(wpath) != Gdiplus::Ok) {
            free(wpath);
            return false;
        }
        free(wpath);
        return true;
    }

    // Draw text with loaded TTF font
    static void _cgame_text_draw_complex_impl(const char* text, int x, int y,
                                              const wchar_t* fontName, float size,
                                              int r, int g, int b) {
        if (!_cgame_memdc || !text || !fontName) return;

        Graphics gdi(_cgame_memdc);
        SolidBrush brush(Color(255, r, g, b));
        FontFamily family(fontName, &_cgame_font_collection);
        Font font(&family, size, FontStyleRegular, UnitPixel);

        WCHAR wtext[512];
        MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, 512);
        gdi.DrawString(wtext, -1, &font, PointF((REAL)x, (REAL)y), &brush);
    }




    // =========================
    // Keyboard helpers
    // =========================
    static bool _cgame_key_pressed_impl(int key) {
        if ((key & 0xFF) >= 512) return false;
        return _cgame_key_state[key & 0xFF];
    }

    static bool _cgame_key_just_pressed_impl(int key) {
        if ((key & 0xFF) >= 512) return false;
        SHORT result = GetAsyncKeyState(key);
        bool now = (result & 0x8000) != 0;
        bool was = _cgame_key_prev_async[key & 0xFF];
        _cgame_key_prev_async[key & 0xFF] = now;
        return now && !was;
    }

    static bool _cgame_key_just_released_impl(int key) {
        if ((key & 0xFF) >= 512) return false;
        SHORT result = GetAsyncKeyState(key);
        bool now = (result & 0x8000) != 0;
        bool was = _cgame_key_prev_async[key & 0xFF];
        _cgame_key_prev_async[key & 0xFF] = now;
        return !now && was;
    }

    static WPARAM _cgame_key_last_impl(void) { return _cgame_last_key; }

    // =========================
    // Timing helpers
    // =========================
    static unsigned long _cgame_start_ticks = 0;
    static unsigned long _cgame_frame_delay = 0;

    static CGameTicks _cgame_time_get_ticks_impl(void) {
        CGameTicks t;
        t.ms = GetTickCount() - _cgame_start_ticks;
        return t;
    }

    static void _cgame_time_set_fps_impl(int fps) {
        if (fps > 0) {
            _cgame_frame_delay = 1000 / fps;
        } else {
            _cgame_frame_delay = 0;
        }
    }

    static void _cgame_time_delay_if_needed_impl(CGameTicks frame_start) {
        if (_cgame_frame_delay > 0) {
            unsigned long elapsed = (GetTickCount() - _cgame_start_ticks) - frame_start.ms;
            if (elapsed < _cgame_frame_delay) {
                Sleep(_cgame_frame_delay - elapsed);
            }
        }
    }



    // =========================
    // Mouse helpers
    // =========================
    static bool _cgame_mouse_pressed_impl(int button) {
        if (button < 1 || button > 5) return false;
        return _cgame_mouse_state[button];
    }

    static bool _cgame_mouse_just_pressed_impl(int button) {
        if (button < 1 || button > 5) return false;
        bool now = _cgame_mouse_state[button];
        bool was = _cgame_mouse_prev[button];
        _cgame_mouse_prev[button] = now;
        return now && !was;
    }

    static bool _cgame_mouse_just_released_impl(int button) {
        if (button < 1 || button > 5) return false;
        bool now = _cgame_mouse_state[button];
        bool was = _cgame_mouse_prev[button];
        _cgame_mouse_prev[button] = now;
        return !now && was;
    }

    static void _cgame_mouse_get_pos_impl(int* x, int* y) {
        if (x) *x = _cgame_mouse_x;
        if (y) *y = _cgame_mouse_y;
    }

    static void _cgame_mouse_set_pos_impl(int x, int y) {
        _cgame_mouse_x = x;
        _cgame_mouse_y = y;
        if (_cgame_screen.hwnd) {
            POINT pt = {x, y};
            ClientToScreen(_cgame_screen.hwnd, &pt);
            SetCursorPos(pt.x, pt.y);
        }
    }

    static int _cgame_mouse_get_wheel_impl(void) {
        int delta = _cgame_mouse_wheel;
        _cgame_mouse_wheel = 0; // reset after read
        return delta;
    }

    // =========================
    // Customization helpers
    // =========================

    static void _cgame_display_set_title(const char* title) {
        if (!title) return;

        // Ensure buffer is cleared
        _cgame_window_title[0] = L'\0';

        int len = MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,   // stricter conversion
            title, -1,
            _cgame_window_title,
            255
        );

        if (len == 0) {
            // conversion failed, fallback
            wcscpy_s(_cgame_window_title, 256, L"CGame Window");
        }

        if (_cgame_screen.hwnd) {
            if (!SetWindowTextW(_cgame_screen.hwnd, _cgame_window_title)) {
                MessageBoxW(NULL, L"SetWindowTextW failed!", L"CGame Debug", MB_OK);
            }
        }
    }


    static bool _cgame_display_set_icon(const char* path) {
        if (!path) return false;

        // Load .ico from file
        int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
        WCHAR* wpath = (WCHAR*)malloc(wlen * sizeof(WCHAR));
        MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

        HICON hIcon = (HICON)LoadImageW(NULL, wpath, IMAGE_ICON, 0, 0,
                                        LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
        free(wpath);

        if (!hIcon) return false;

        _cgame_window_icon = hIcon;
        if (_cgame_screen.hwnd) {
            SendMessage(_cgame_screen.hwnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            SendMessage(_cgame_screen.hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        return true;
    }

    // =========================
    // Public API
    // =========================
    static struct {
        void (*init)(void);
        void (*quit)(void);

        struct {
            CGameScreen (*set_mode)(int w, int h, int flags);
            void (*set_bgcolor)(int r, int g, int b);
            void (*clear)(void);
            void (*flip)(void);
            int  (*get_width)(void);
            int  (*get_height)(void);
            void (*set_title)(const char* title);
            bool (*set_icon)(const char* path);
            bool (*set_icon_from_image)(const CGameImage* img);
        } display;

        // console reset and set color
        struct {
            void (*set_color)(int fg, int bg);
            void (*reset_color)(void);
        } console;

        // msg box show 
        struct {
            int (*show)(const wchar_t* title, const wchar_t* text, unsigned int type);
        } messagebox;


        struct {
            int (*get)(void);
        } event;

        // keyboard api
        struct {
            bool (*pressed)(int key);
            bool (*just_pressed)(int key);
            bool (*just_released)(int key);
            WPARAM (*get_last)(void);
        } key;

        // mouse api
        struct {
            bool (*pressed)(int button);
            bool (*just_pressed)(int button);
            bool (*just_released)(int button);
            void (*get_pos)(int* x, int* y);
            void (*set_pos)(int x, int y);
            int  (*get_wheel)(void);
        } mouse;

        // polygon draw api
        struct {
            void (*rect)(int x, int y, int w, int h, int r, int g, int b);
            void (*fill_rect)(int x, int y, int w, int h, int r, int g, int b);
            void (*polygon)(const int* xs, const int* ys, int count, int r, int g, int b);
            void (*fill_polygon)(const int* xs, const int* ys, int count, int r, int g, int b);
            void (*rounded_rect)(int x, int y, int w, int h, int radius, int border_width,
                                 int r, int g, int b);
            void (*rounded_fill_rect)(int x, int y, int w, int h, int radius,
                                      int r, int g, int b);
        } draw;


        // image api
        struct {
            CGameImage (*load)(const char* path);
            void (*unload)(CGameImage* img);
            void (*draw)(const CGameImage* img, int x, int y);
            void (*draw_scaled)(const CGameImage* img, int x, int y, int w, int h);
            CGameImage (*resize)(const CGameImage* src, int w, int h);
            bool (*flip_horizontal)(CGameImage* img);
            bool (*flip_vertical)(CGameImage* img);
            int (*get_width)(const CGameImage* img);
            int (*get_height)(const CGameImage* img);
            int (*get_channels)(const CGameImage* img);
        } image;

        // timing
        struct {
            CGameTicks (*get_ticks)(void);
            void  (*set_fps)(int fps);
            void  (*delay_if_needed)(CGameTicks frame_start);
        } time;


        // text rendering
        struct {
            void (*draw)(const char* text, int x, int y, int r, int g, int b);
            void (*draw_complex)(const char* text, int x, int y,
                                 const wchar_t* fontName, float size,
                                 int r, int g, int b);
            bool (*load_font)(const char* path);
        } text;




        struct {
            int (*get)(void);
        } event2;

        int QUIT;
        int VIDEORESIZE;
        int KEYDOWN;
        int KEYUP;

        int K_a, K_b, K_c, K_d, K_e, K_f, K_g, K_h, K_i, K_j, K_k, 
        K_l, K_m, K_n, K_o, K_p, K_q, K_r, K_s, K_t, K_u, K_v, 
        K_w, K_x, K_y, K_z,
        K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
        K_SPACE, K_RETURN, K_ESCAPE, K_LEFT, K_RIGHT, K_UP, K_DOWN;
    } cgame;

    // =========================
    // Initialize function table
    // =========================
    static void _cgame_init_struct(void) {
        cgame.init                        = _cgame_init;
        cgame.quit                        = _cgame_quit;

        // window cutomization API bindings
        cgame.display.set_title           = _cgame_display_set_title;
        cgame.display.set_icon            = _cgame_display_set_icon;
        cgame.display.set_icon_from_image = _cgame_display_set_icon_from_image;
        cgame.display.set_mode            = _cgame_set_mode_impl;
        cgame.display.set_bgcolor         = _cgame_display_set_bgcolor;
        cgame.display.clear               = _cgame_display_clear_impl;
        cgame.display.flip                = _cgame_display_flip_impl;
        cgame.display.get_width           = _cgame_display_get_width;
        cgame.display.get_height          = _cgame_display_get_height;
        cgame.event.get                   = _cgame_event_get;

        //  console 
        cgame.console.set_color           = _cgame_console_set_color;
        cgame.console.reset_color         = _cgame_console_reset_color;
        // msg box
        cgame.messagebox.show             = _cgame_message_box;


        // keyboard API bindings
        cgame.key.pressed                 = _cgame_key_pressed_impl;
        cgame.key.just_pressed            = _cgame_key_just_pressed_impl;
        cgame.key.just_released           = _cgame_key_just_released_impl;
        cgame.key.get_last                = _cgame_key_last_impl;

        // mouse API bindings
        cgame.mouse.pressed               = _cgame_mouse_pressed_impl;
        cgame.mouse.just_pressed          = _cgame_mouse_just_pressed_impl;
        cgame.mouse.just_released         = _cgame_mouse_just_released_impl;
        cgame.mouse.get_pos               = _cgame_mouse_get_pos_impl;
        cgame.mouse.set_pos               = _cgame_mouse_set_pos_impl;
        cgame.mouse.get_wheel             = _cgame_mouse_get_wheel_impl;

        // polygon drawing API bindings
        cgame.draw.fill_rect              = _cgame_draw_fillrect_impl;
        cgame.draw.rect                   = _cgame_draw_rect_impl;
        cgame.draw.polygon                = _cgame_draw_polygon_impl;
        cgame.draw.fill_polygon           = _cgame_draw_fill_polygon_impl;
        cgame.draw.rounded_rect           = _cgame_draw_rounded_rect_impl;
        cgame.draw.rounded_fill_rect      = _cgame_draw_rounded_fill_rect_impl;

        // image API bindings
        cgame.image.load                  = _cgame_image_load_impl;
        cgame.image.unload                = _cgame_image_unload_impl;
        cgame.image.draw                  = _cgame_image_draw_impl;
        cgame.image.draw_scaled           = _cgame_image_draw_scaled_impl;
        cgame.image.resize                = _cgame_image_resize_nearest;
        cgame.image.flip_horizontal       = _cgame_image_flip_horizontal_impl;
        cgame.image.flip_vertical         = _cgame_image_flip_vertical_impl;
        cgame.image.get_width             = _cgame_image_get_width_impl;
        cgame.image.get_height            = _cgame_image_get_height_impl;
        cgame.image.get_channels          = _cgame_image_get_channels_impl;

        // timing API bidings
        _cgame_start_ticks                = GetTickCount();
        cgame.time.get_ticks              = _cgame_time_get_ticks_impl;
        cgame.time.set_fps                = _cgame_time_set_fps_impl;
        cgame.time.delay_if_needed        = _cgame_time_delay_if_needed_impl;

        // text API bindings
        cgame.text.draw         = _cgame_text_draw_impl;
        cgame.text.draw_complex = _cgame_text_draw_complex_impl;
        cgame.text.load_font    = _cgame_text_load_font_impl;


        // MAJOR EVENTSSSSSSSSSSS....
        cgame.QUIT               = CGAME_QUIT;
        cgame.VIDEORESIZE        = CGAME_VIDEORESIZE;
        cgame.KEYDOWN            = CGAME_KEYDOWN;
        cgame.KEYUP              = CGAME_KEYUP;

        cgame.K_a = CGAME_K_a;
        cgame.K_b = CGAME_K_b;
        cgame.K_c = CGAME_K_c;
        cgame.K_d = CGAME_K_d;
        cgame.K_e = CGAME_K_e;
        cgame.K_f = CGAME_K_f;
        cgame.K_g = CGAME_K_g;
        cgame.K_h = CGAME_K_h;
        cgame.K_i = CGAME_K_i;
        cgame.K_j = CGAME_K_j;
        cgame.K_k = CGAME_K_k;
        cgame.K_l = CGAME_K_l;
        cgame.K_m = CGAME_K_m;
        cgame.K_n = CGAME_K_n;
        cgame.K_o = CGAME_K_o;
        cgame.K_p = CGAME_K_p;
        cgame.K_q = CGAME_K_q;
        cgame.K_r = CGAME_K_r;
        cgame.K_s = CGAME_K_s;
        cgame.K_t = CGAME_K_t;
        cgame.K_u = CGAME_K_u;
        cgame.K_v = CGAME_K_v;
        cgame.K_w = CGAME_K_w;
        cgame.K_x = CGAME_K_x;
        cgame.K_y = CGAME_K_y;
        cgame.K_z = CGAME_K_z;

        cgame.K_0 = CGAME_K_0;
        cgame.K_1 = CGAME_K_1;
        cgame.K_2 = CGAME_K_2;
        cgame.K_3 = CGAME_K_3;
        cgame.K_4 = CGAME_K_4;
        cgame.K_5 = CGAME_K_5;
        cgame.K_6 = CGAME_K_6;
        cgame.K_7 = CGAME_K_7;
        cgame.K_8 = CGAME_K_8;
        cgame.K_9 = CGAME_K_9;

        cgame.K_SPACE  = CGAME_K_SPACE;
        cgame.K_RETURN = CGAME_K_RETURN;
        cgame.K_ESCAPE = CGAME_K_ESCAPE;
        cgame.K_LEFT   = CGAME_K_LEFT;
        cgame.K_RIGHT  = CGAME_K_RIGHT;
        cgame.K_UP     = CGAME_K_UP;
        cgame.K_DOWN   = CGAME_K_DOWN;
    }

    static int _cgame_dummy_init = (_cgame_init_struct(), 0);

#elif defined (MACOS)
    #include <motivation.h>
#error "cgame.h only supports Windows"
#endif

// -----------------------------------------------------------
// Entry point shim for MinGW when using -municode
// -----------------------------------------------------------
#if defined(_WIN32) && (defined(__MINGW32__) || defined(__MINGW64__)) && !defined(CGAME_NO_WINMAIN_SHIM)
extern "C" int main(void); // forward declare user main()

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd) {
    (void)hInstance; (void)hPrevInstance;
    (void)lpCmdLine; (void)nShowCmd;
    return main(); // call your real main()
}
#endif


#endif // CGAME_H

