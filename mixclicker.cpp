#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<unistd.h>
#include<chrono>
#include<thread>
using namespace std;
int main()
{
    int inp1=0,t=0,countt=1,tag=0;
    cout << "\tWelcome to Khannapong's Auto Clicker";
    cout << "\n----------------------------------------------";
    cout << "\nPress [1] for Left Click\nPress [2] for Right Click";
    cout << "\n----------------------------------------------\n";
    while (true)
    {
        if(GetAsyncKeyState(0x31) & 0x8000 )
        {
            cout << "\nClick = LEFT";
            inp1=1;
            break;
        }
        else if (GetAsyncKeyState(0x32) & 0x8000)
        {
            cout << "\nClick = RIGHT";
            inp1=2;
            break;
        }
    }
    getch();
    cout << "\nClick Interval(Second) = ";
    cin >> t;
    cout << "\n----------------------------------------------";
    cout << "\nPress \"F6\" to Start Clicking\n or Stop Clicking\n";
    while (tag==0)
    {
        if (inp1==1)
        {
        if(GetAsyncKeyState(VK_F6) & 0x8000)
        {
            cout << "Program is starting...";
            sleep(1);
            while(tag==0)
            {
                cout<< "\nClick(s) Count = " << countt << "\n";
                countt++;
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                this_thread::sleep_for(chrono::microseconds(100000));
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                auto start = std::chrono::system_clock::now();
                while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() -start).count() != t)
                {
                    //cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() <<"\n";
                    if(GetAsyncKeyState(VK_F6) & 0x8000)
                    {
                        tag=1;
                        break;
                    }
                }
            }
        }
        }
        else
        {
        if(GetAsyncKeyState(VK_F6) & 0x8000)
        {
            cout << "Program is starting...";
            sleep(1);
            while(tag==0)
            {
                cout<< "\nClick(s) Count = " << countt << "\n";
                countt++;
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                this_thread::sleep_for(chrono::microseconds(100000));
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                auto start = std::chrono::system_clock::now();
                while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() -start).count() != t)
                {
                    //cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() <<"\n";
                    if(GetAsyncKeyState(VK_F6) & 0x8000)
                    {
                        tag=1;
                        break;
                    }
                }
            }
        }
        }
    }
    cout << "\n----------------------------------------------";
    cout << "\n\tSee you again next time!\n";
    cout << "----------------------------------------------\n";
    system("pause");
}