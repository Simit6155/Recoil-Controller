#include <windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <conio.h>

class Mouse {
public:
	void move(short x, short y) {
		mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	}

	void click(short delay_ms) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
};

Mouse m;

int recoil_strength = 5;
int recoil_x[] = { 0, 0, 0, 0, 0 };
int recoil_y[] = { 1, 2, 3, 4, 5 };
int pattern_size = sizeof(recoil_y) / sizeof(recoil_y[0]);

#define RESET_COLOR "\033[0m"
#define GREEN "\033[1;32m"
#define GOLD "\033[1;33m"  
#define LIGHT_GRAY "\033[0;37m"  
#define DARK_GRAY "\033[1;30m"

void show_banner() {
    system("cls");


    std::cout << GOLD << "============================" << RESET_COLOR << "\n";
    std::cout << GREEN << "     SIMIT RECOIL" << RESET_COLOR << "\n";
    std::cout << GOLD << "============================" << RESET_COLOR << "\n";
    std::cout << LIGHT_GRAY << "\nCurrent Recoil Strength: " << GREEN << recoil_strength << "/10" << RESET_COLOR << "\n";


    std::cout << LIGHT_GRAY << "Strength Level: [" << RESET_COLOR;
    for (int i = 0; i < 10; i++) {
        if (i < recoil_strength) {
            std::cout << GREEN << "#";
        }
        else {
            std::cout << LIGHT_GRAY << "-";
        }
    }
    std::cout << LIGHT_GRAY << "]" << RESET_COLOR << "\n";

    std::cout << "\n" << LIGHT_GRAY << "Press number keys 1-10 to change strength." << RESET_COLOR << "\n";
    std::cout << LIGHT_GRAY << "Press 'q' to quit." << RESET_COLOR << "\n";
}


void recoil_thread() {
    int counter = 0;

    for (;;) {
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && (GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {

            if (counter < pattern_size) {

                m.move(recoil_x[counter] * recoil_strength, recoil_y[counter] * recoil_strength);
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
                counter++;
            }
            if (counter >= pattern_size) {
                counter = 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void input_thread() {
    while (true) {
        if (_kbhit()) {
            char ch = _getch();


            if (ch == 'q' || ch == 'Q') {
                break;
            }


            if (ch >= '1' && ch <= '9') {
                recoil_strength = ch - '0';
                show_banner();
            }
            else if (ch == '0') {
                recoil_strength = 10;
                show_banner();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main() {
    show_banner();


    std::thread recoil_thread_instance(recoil_thread);
    std::thread input_thread_instance(input_thread);


    input_thread_instance.join();
    recoil_thread_instance.join();

    return 0;
}