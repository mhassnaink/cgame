# cgame

**cgame** is a lightweight, header-only, CPU-based C++ framework for building fast and efficient graphical applications and custom GUI systems.  
It was designed and developed by **M. Hassnain K** over a focused two-week period for Windows, providing developers with full control, minimal dependencies, and game-level performance.

---

## ⚙️ Features
- Header-only, pure C++ (no dependencies)
- CPU-based rendering (no GPU requirement)
- Custom GUI and rendering system
- Cross-compiler support (MinGW & MSVC)
- Future roadmap for Linux and macOS

---

## 🧩 Example
```cpp
#include <cgame/cgame.h>

int main() {
    cgame.init();
    cgame.display.set_mode(800, 600, CGAME_RESIZABLE);
    cgame.display.set_title("Hello from cgame!");

    while (true) {
        if (cgame.event.get() == cgame.QUIT) break;
        cgame.draw.fill_rect(100, 100, 200, 150, 255, 0, 0);
    }

    cgame.quit();
}
