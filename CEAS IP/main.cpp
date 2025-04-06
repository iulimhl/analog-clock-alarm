#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <winbgim.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>
#include <vector>
#include <sstream>
#include <windows.h>

using namespace std;

#define PI 3.142857
#define pages 4
#define functions 9

float multiplicationFactor = 1;
float topBarTop = 1.5, topBarBottom = 4, topBarUnits = 32;

/* Cnvention for array[page] == page and function_nr coleration;

    1 = analog clock funtion;
    2 = digital clock funtion;
    3 = stopwatch
    4 = alarm menu
    5 = settings menu
    6 = add alarm
    7 = start/pause stopwatch
    8 = stop/restart stopwatch
*/


int analog_code = 1, home_code=0, digital_code = 2, stopwatch_code = 3,  alarm_code= 4, setTimeCode = 5, exitCode = 6, playPauseCode = 7, stopCode = 8;
int page=0;
bool showHowToUseAlarm = false;
bool alarmaActivaVizibila = false;


struct Alarma
{
    int ora, minut;
    bool activa;
    time_t timpDezactivare;
};

struct page_button
{
    int left, right, top, bottom, function_code;
};

page_button buttons_array[functions];
page_button welcomePageButtonsArray[functions];

Alarma alarme[10];
int numarAlarme = 0;
int ora = 0, minut = 0, butonSelectat = -1;
bool selectatOra = true, listaAfisata = false;

int LightBlue=COLOR(175,221,255), SkyBlue=COLOR(110,153,202);
int babyPinkColor = COLOR(255, 182, 193), hotpink = COLOR(255, 105, 180), barbiepink = COLOR(255, 20, 147), pink = COLOR(219, 112, 147);
int butoaneSUS=COLOR(255, 178, 208), butoaneSUS2=COLOR(178, 255, 226);

void tickSound()
{
    PlaySound(TEXT("tick.wav"), NULL, SND_ASYNC);
}


void drawClockHand(double length, double thickness, int color, int value, double maxValue, int xCenter, int yCenter)
{
    setcolor(color);
    setlinestyle(0, 0, thickness);
    line(xCenter, yCenter, xCenter + length * sin(value * PI / maxValue), yCenter - length * cos(value * PI / maxValue));
}


void adaugaAlarma(int ora, int minut)
{
    for (int i=0; i<numarAlarme; i++)
    {
        if (alarme[i].ora==ora && alarme[i].minut == minut)
            return;
    }
    alarme[numarAlarme++] = {ora, minut, false, 0};
}


void deseneazaButon(int x, int y, int latime, int inaltime, const char* text, bool selectat)
{
    char b[20];
    settextjustify(1,1);
    strcpy(b, text);
    setfillstyle(SOLID_FILL, selectat ? hotpink : barbiepink);
    bar(x, y, x + latime, y + inaltime);
    rectangle(x, y, x + latime, y + inaltime);
    setbkcolor(selectat ? hotpink : barbiepink);
    setcolor(WHITE);
    outtextxy(x + latime / 2, y + inaltime / 2, b);
}


void buttons_top_bar(int functionCalledNr, int app_width, int app_height, int color) // for pages: analog, digital, alarms, stopwatch.
{
    int depth = 5;
    int textFont = 1;
    int highlightColor = 7;
    int buttonWidthUnits = 5, buttonUnitsBetween = 2.8, totalUnits = buttonUnitsBetween * 5 + buttonWidthUnits * 4;
    int buttonWidth = app_width * buttonWidthUnits / totalUnits, buttonsSpaceBetween = app_width * buttonUnitsBetween / totalUnits;

    int bar_top = app_height * topBarTop / topBarUnits, bar_bottom = app_height * topBarBottom / topBarUnits;
    int bar_height_middle = (bar_top + bar_bottom) /2;
    int button_width_middle;
    int culoarenou=(182, 182, 180);
    setcolor(culoarenou);
    settextjustify(1,1);
    settextstyle(3, HORIZ_DIR, textFont);
    setlinestyle(0, 1, 2);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, color);
    setbkcolor(color);

    //home
    int btnWidth = 150;
    int btnHeight = 40;
    int btnX = app_width - btnWidth -20;
    int btnY = app_height*4/3 - 50;


    bar3d(btnX, btnY, btnX + btnWidth, btnY + btnHeight, depth, 1);
    outtextxy((btnX + btnX + btnWidth) / 2, (btnY + btnY + btnHeight) / 2, "Go back home");
    buttons_array[home_code].left = btnX;
    buttons_array[home_code].right = btnX + btnWidth;
    buttons_array[home_code].top = btnY;
    buttons_array[home_code].bottom = btnY + btnHeight;
    buttons_array[home_code].function_code = home_code;

    setfillstyle(SOLID_FILL, color);
    setbkcolor(color);

    //alarm.
    int alarm_code = 4;
    buttons_array[alarm_code].left = app_width * buttonUnitsBetween / totalUnits;
    buttons_array[alarm_code].top = bar_top;
    buttons_array[alarm_code].right = buttons_array[alarm_code].left + buttonWidth;
    buttons_array[alarm_code].bottom = bar_bottom;
    buttons_array[alarm_code].function_code = alarm_code;

    if (buttons_array[alarm_code].function_code == functionCalledNr)
    {
        setfillstyle(SOLID_FILL, hotpink);
        setbkcolor(hotpink);
    }
    bar3d(buttons_array[alarm_code].left, bar_top, buttons_array[alarm_code].right, bar_bottom, depth, 1);
    outtextxy((buttons_array[alarm_code].left + buttons_array[alarm_code].right)/2, bar_height_middle, "Alarm");

    setfillstyle(SOLID_FILL, color);
    setbkcolor(color);

    //analog.;
    buttons_array[analog_code].left = buttons_array[alarm_code].right + buttonsSpaceBetween;
    buttons_array[analog_code].top = bar_top;
    buttons_array[analog_code].right = buttons_array[analog_code].left + buttonWidth;
    buttons_array[analog_code].bottom = bar_bottom;
    buttons_array[analog_code].function_code = analog_code;

    if (buttons_array[analog_code].function_code == functionCalledNr)
    {
        setfillstyle(SOLID_FILL, hotpink);
        setbkcolor(hotpink);
    }
    bar3d(buttons_array[analog_code].left, bar_top, buttons_array[analog_code].right, bar_bottom, depth,1);
    outtextxy((buttons_array[analog_code].left + buttons_array[analog_code].right)/2, bar_height_middle, "Analog");

    setfillstyle(SOLID_FILL, color);
    setbkcolor(color);


}


int detect_button_and_function(int isPageWelcomePage, int x_mouse, int y_mouse)
{
    for (int i = 0; i < functions; i++)
    {
        int left, right, top, bottom, function_code;

        if (isPageWelcomePage)
        {
            left = welcomePageButtonsArray[i].left;
            right = welcomePageButtonsArray[i].right;
            top = welcomePageButtonsArray[i].top;
            bottom = welcomePageButtonsArray[i].bottom;
            function_code = welcomePageButtonsArray[i].function_code;
        }
        else
        {
            left = buttons_array[i].left;
            right = buttons_array[i].right;
            top = buttons_array[i].top;
            bottom = buttons_array[i].bottom;
            function_code = buttons_array[i].function_code;
        }

        if (x_mouse >= left && x_mouse <= right && y_mouse >= top && y_mouse <= bottom)
        {
            return function_code;
        }
    }

    return -1;
}



void deseneazaChenare(int ora, int minut, int appWidth, int appHeight)
{
    setbkcolor(butoaneSUS);
    cleardevice();
    settextjustify(1,1);
    setcolor(barbiepink);
    settextstyle(4, HORIZ_DIR, 1);
    char oraText[] = "Hour";
    char minutText[] = "Minute";
    outtextxy(100, 130, oraText);
    outtextxy(250, 130, minutText);

    setfillstyle(SOLID_FILL, selectatOra ? hotpink : barbiepink);

    bar(50, 150, 150, 200);
    rectangle(50, 150, 150, 200);

    setfillstyle(SOLID_FILL, !selectatOra ? hotpink : barbiepink);

    bar(200, 150, 300, 200);
    rectangle(200, 150, 300, 200);

    setcolor(WHITE);
    char oraStr[3], minutStr[3];
    sprintf(oraStr, "%02d", ora);
    sprintf(minutStr, "%02d", minut);

    setbkcolor(selectatOra ? hotpink : barbiepink);
    outtextxy(100, 175, oraStr);
    setbkcolor(!selectatOra ? hotpink : barbiepink);
    outtextxy(250, 175, minutStr);

    deseneazaButon(50, 250, 220, 50, "Set alarm", butonSelectat == 0);
    deseneazaButon(50, 350, 220, 50, "New alarm", butonSelectat == 1);
    deseneazaButon(50, 450, 220, 50, "Alarm's list", butonSelectat == 2);

    buttons_top_bar(alarm_code, appWidth, appHeight, butoaneSUS);
}


void verificaAlarme()
{
    int xCenter = getmaxwidth()/4;
    time_t acum = time(0);
    struct tm *timpAcum = localtime(&acum);

    for (int i = 0; i < numarAlarme; i++)
    {
        Alarma &alarma = alarme[i];
        if (timpAcum->tm_hour == alarma.ora && timpAcum->tm_min == alarma.minut && !alarma.activa)
        {
            if (difftime(acum, alarma.timpDezactivare) > 60)
            {
                PlaySound(TEXT("alarma.wav"), NULL, SND_ASYNC);
                alarma.activa = true;
                setactivepage(page);
                setbkcolor(barbiepink);
                setcolor(WHITE);
                outtextxy(xCenter, 480, "Click 'o' to stop the alarm.");
                outtextxy(xCenter, 500, "Click 's' to snooze the alarm.");


                page = 1 - page;
                setactivepage(page);
                setbkcolor(barbiepink);
                setcolor(WHITE);
                outtextxy(xCenter, 480, "Click 'o' to stop the alarm.");
                outtextxy(xCenter, 500, "Click 's' to snooze the alarm.");
                alarmaActivaVizibila = true;

                delay(3000);

            }
        }
    }
}

void drawAlarmHowToUse(int appWidth, int appHeight)
{
    int xCenter = getmaxwidth() / 4;
    const int chenarLatime = 380;
    const int chenarTop = 20;
    const int chenarBottom = 170;
    const int xText = xCenter + chenarLatime / 2;

    //FUNDAL+CHENAR
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, hotpink);
    bar(xCenter, chenarTop, xCenter + chenarLatime, chenarBottom);
    rectangle(xCenter, chenarTop, xCenter + chenarLatime, chenarBottom);

    settextjustify(CENTER_TEXT, TOP_TEXT);

    int y = 40;

    settextstyle(BOLD_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    setbkcolor(hotpink);
    outtextxy(xText, y, "-------How to use the alarm-------");
    y += 20;

    char instructiuni[5][100] =
{
    "->Up/Down key: Change hour or minute",
    "->Left/Right key: Switch between hour and minute",
    "->Click 'Set alarm' to save",
    "->Click 'New alarm' to reset hour and minute",
    "->Click 'Alarm list' to view alarms"
};


    settextstyle(3, HORIZ_DIR, 1);

    for (int i = 0; i < 5; i++)
    {
        int culoareFundal = (i % 2 == 0) ? hotpink : hotpink;

        setfillstyle(SOLID_FILL, culoareFundal);
        bar(xCenter + 1, y, xCenter + chenarLatime, y + 20);

        setbkcolor(culoareFundal);
        outtextxy(xText, y, instructiuni[i]);

        y += 20;
    }
}


void afiseazaListaAlarme()
{
    int xCenter = getmaxwidth() / 4;
    const int chenarLatime = 380;
    const int chenarTop=190;
    const int xText = xCenter + chenarLatime / 2; // centru orizontal în chenar
    const int chenarBottom=520;

    setfillstyle(SOLID_FILL, hotpink);
    bar(xCenter, chenarTop, xCenter + chenarLatime, chenarBottom);
    rectangle(xCenter, chenarTop, xCenter + chenarLatime, chenarBottom);

    settextstyle(4, HORIZ_DIR, 1);
    setcolor(WHITE);
    setbkcolor(hotpink);
    settextjustify(CENTER_TEXT, TOP_TEXT);

    int y = 210;
    outtextxy(xText, y, "Alarm's list:");
    y += 20;

    for (int i = 0; i < numarAlarme; i++)
    {
        Alarma &alarma = alarme[i];
        char timeString[6];
        sprintf(timeString, "%02d:%02d", alarma.ora, alarma.minut);
        outtextxy(xText, y, timeString);
        y += 20;
    }

    outtextxy(xText, 480, "Press 'Esc' to exit");
}



int gestionareInput(int appWidth, int appHeight)
{
    int x_mouse, y_mouse;

    if (kbhit())
    {
        char ch = getch();
        if (ch == 27 && listaAfisata)  //esc
        {
            cleardevice();
            deseneazaChenare(ora, minut, getmaxwidth()/2, getmaxheight()/2);
            listaAfisata = false;
        }
        else if (ch == 'o'&&alarmaActivaVizibila)
        {
            time_t acum = time(0);
            for (int i = 0; i < numarAlarme; i++)
            {
                Alarma &alarma = alarme[i];
                if (alarma.activa)
                {
                    alarma.activa = false;
                    PlaySound(NULL, NULL, 0);
                    alarma.timpDezactivare = acum;
                    alarmaActivaVizibila = false;
                    break;
                }
            }
        }
        else if (ch == 's'&&alarmaActivaVizibila)
        {
            time_t acum = time(0);
            for (int i = 0; i < numarAlarme; i++)
            {
                Alarma &alarma = alarme[i];
                if (alarma.activa)
                {
                    alarma.activa = false;
                    PlaySound(NULL, NULL, 0);
                    Sleep(300000);
                    PlaySound(TEXT("alarma.wav"), NULL, SND_ASYNC);
                    alarma.timpDezactivare = acum;
                    alarmaActivaVizibila=false;
                    break;
                }
            }
        }

        else if (ch == 0)
        {
            ch = getch();
            switch (ch)
            {
            case 72:
                selectatOra ? ora = (ora + 1) % 24 : minut = (minut + 1) % 60;
                break;
            case 80:
                selectatOra ? ora = (ora + 23) % 24 : minut = (minut + 59) % 60;
                break;
            case 75:
                selectatOra = true;
                break;
            case 77:
                selectatOra = false;
                break;
            }
            deseneazaChenare(ora, minut, getmaxwidth()/2, getmaxheight()/2);
        }
    }

    int x, y, function_code = -1;

    if (ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        x = mousex();
        y = mousey();
        x_mouse = x;
        y_mouse = y;

        if (x >= 50 && x <= 250 && y >= 250 && y <= 300)
        {
            butonSelectat = 0;
            adaugaAlarma(ora, minut);
        }
        else if (x >= 50 && x <= 250 && y >= 350 && y <= 400)
        {
            butonSelectat = 1;
            ora = 0;
            minut = 0;
            selectatOra = true;
        }
        else if (x >= 50 && x <= 250 && y >= 450 && y <= 500)
        {
            butonSelectat = 2;
            cleardevice();
            afiseazaListaAlarme();
            listaAfisata = true;
        }

        else
        {
            butonSelectat = -1;
        }
        deseneazaChenare(ora, minut, getmaxwidth()/2, getmaxheight()/2);


        function_code = detect_button_and_function(0, x_mouse, y_mouse);

        if (function_code == home_code)
            return home_code;


    }

    if ((function_code > -1) && (function_code < 5))
    {
        //closegraph();
        cleardevice();
        return function_code;
    }
    return -1;
}


int alarm_menu (int maxWidth, int maxHeight)
{
    int function_code = -1;

    int appWidth = maxWidth/2, appHeight = maxHeight/2;

    setcurrentwindow(page);
    setbkcolor(hotpink);
    deseneazaChenare(ora, minut, appWidth, appHeight);

    while (true)
    {
        setactivepage(page);
        cleardevice();
        deseneazaChenare(ora, minut, appWidth, appHeight);
        verificaAlarme();
        function_code = gestionareInput(appWidth, appHeight);

        drawAlarmHowToUse(appWidth, appHeight);

        if(listaAfisata)
            afiseazaListaAlarme();


        setvisualpage(page);
        page = 1 - page;

        if ((function_code > -1) && (function_code < 5))
        {
            //closegraph();
            return function_code;
            //break;
        }

        delay(200);
    }


}


int welcome_page(int max_width, int max_height, int currentWindowNr)
{
    int app_width = max_width / 2, app_height = max_height / 2;
    int function_code = -1;
    int button_width = app_width / 5;
    int button_height = app_height / 6;
    int spacing_x = app_width / 20;

    int base_top = app_height * 2 / 3;

    int xcenter = app_width / 2, ycenter = app_height / 2;

    setcurrentwindow(currentWindowNr);

    setbkcolor(butoaneSUS);
    cleardevice();

    settextjustify(1, 1);
    settextstyle(4, HORIZ_DIR, 9);
    setcolor(0);

    //temp for static final text position
    settextstyle(4, HORIZ_DIR, 6);
    setcolor(barbiepink);
    outtextxy(xcenter, ycenter/3, "   Analog clock and alarm");
    settextstyle(4, HORIZ_DIR, 4);
    setcolor(WHITE);
    outtextxy(xcenter, ycenter, "Pick an option:");


    //buttons
    setfillstyle(SOLID_FILL, butoaneSUS);
    setlinestyle(0, 0, 3);
    setbkcolor(butoaneSUS);
    settextstyle(4, HORIZ_DIR, 3);

    //alarm button
    setcolor(barbiepink);
    int alarm_border_left = xcenter - button_width - spacing_x/2;
    int alarm_border_top = base_top;
    int alarm_border_right = alarm_border_left + button_width;
    int alarm_border_bottom = alarm_border_top + button_height;
    bar3d(alarm_border_left, alarm_border_top, alarm_border_right, alarm_border_bottom, 6, 1);
    setcolor(WHITE);
    outtextxy((alarm_border_left + alarm_border_right)/2, (alarm_border_top + alarm_border_bottom)/2, "Alarm");

    welcomePageButtonsArray[alarm_code].left = alarm_border_left;
    welcomePageButtonsArray[alarm_code].top = alarm_border_top;
    welcomePageButtonsArray[alarm_code].right = alarm_border_right;
    welcomePageButtonsArray[alarm_code].bottom = alarm_border_bottom;
    welcomePageButtonsArray[alarm_code].function_code = alarm_code;

    //analog buttom
    setcolor(barbiepink);
    int analog_border_left = xcenter + spacing_x/2;
    int analog_border_top = base_top;
    int analog_border_right = analog_border_left + button_width;
    int analog_border_bottom = analog_border_top + button_height;
    bar3d(analog_border_left, analog_border_top, analog_border_right, analog_border_bottom, 6, 1);
    setcolor(WHITE);
    outtextxy((analog_border_left+analog_border_right)/2, (analog_border_top+analog_border_bottom)/2, "Analog");

    welcomePageButtonsArray[analog_code].left = analog_border_left;
    welcomePageButtonsArray[analog_code].top = analog_border_top;
    welcomePageButtonsArray[analog_code].right = analog_border_right;
    welcomePageButtonsArray[analog_code].bottom = analog_border_bottom;
    welcomePageButtonsArray[analog_code].function_code = analog_code;

    setbkcolor(15);
    settextstyle(4, HORIZ_DIR, 4);


    //mouse coordinates
    int x_mouse, y_mouse;

    do
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x_mouse = mousex();
            y_mouse = mousey();

            function_code = detect_button_and_function(1, x_mouse, y_mouse);
        }
    }
    while (function_code < 0 || function_code > 4);

    //closegraph();
    //cleardevice();
    return function_code;
}

int analog_clock(int max_width, int max_height, int currentWindowNr)
{
    int x_mouse = -1, y_mouse = -1;
    int function_code = -1;
    int functionNr = 1; // analog_clock function_code
    int appWidth = max_width /2, appHeight = max_height/2;
    int lastSecond = -1;

    int babyPinkColor = COLOR(255, 182, 193), hotpink = COLOR(255, 105, 180), barbiepink = COLOR(255, 20, 147), pink = COLOR(219, 112, 147);

    setcurrentwindow(currentWindowNr);

    setbkcolor(butoaneSUS);
    cleardevice();
    settextjustify(1,1);
    buttons_top_bar(analog_code, appWidth, appHeight, butoaneSUS);
    int xCenter = appWidth / 2, yCenter = appHeight - (appHeight * (1 - topBarBottom/topBarUnits)) /2;

    // desenarea numerelor pe cadran
    setcolor(hotpink);
    setbkcolor(butoaneSUS);
    settextstyle(4, HORIZ_DIR, 4);
    for (int i = 1; i <= 12; i++)
    {
        char numStr[3];
        sprintf(numStr, "%d", i);
        outtextxy(xCenter + (appHeight - yCenter) * 0.74 * sin(i * PI / 6), yCenter*1.02 - (appHeight - yCenter) * 0.74 * cos(i * PI / 6), numStr);
    }

    // desenarea cadranului ceasului
    setcolor(hotpink);
    setlinestyle(SOLID_LINE, 0, 11);
    circle(xCenter, yCenter, (appHeight - yCenter) * 0.87);
    setcolor(barbiepink);
    setlinestyle(SOLID_LINE, 0, 2);
    circle(xCenter, yCenter, (appHeight - yCenter) * 0.89);

    time_t now;
    tm *ltm;
    setlinestyle(SOLID_LINE, 0, 2);

    int R = (appHeight - yCenter) * 0.47;
    int r = (appHeight - yCenter) * 0.34;

    while (1)
    {
        // obtinerea timpului curent
        now = time(0);
        ltm = localtime(&now);

        setcolor(butoaneSUS);
        circle(xCenter, yCenter, (appHeight - yCenter)*0.4);
        floodfill(xCenter, yCenter, hotpink);



        setcolor(butoaneSUS);
        setfillstyle(SOLID_FILL, butoaneSUS);

        setcolor(hotpink);
        setfillstyle(SOLID_FILL, hotpink);
        circle(xCenter, yCenter, 6);
        floodfill(xCenter, yCenter, hotpink);

        if (ltm->tm_sec == 59 || ltm->tm_sec == 00)
        {
            drawClockHand((appHeight - yCenter)*0.37, 12, butoaneSUS, ltm->tm_hour * 5-1, 30, xCenter, yCenter);
            drawClockHand((appHeight - yCenter)*0.50, 9, butoaneSUS, ltm->tm_min-1, 30, xCenter, yCenter);
            drawClockHand((appHeight - yCenter)*0.37, 12, butoaneSUS, ltm->tm_hour * 5, 30, xCenter, yCenter);
            drawClockHand((appHeight - yCenter)*0.50, 9, butoaneSUS, ltm->tm_min, 30, xCenter, yCenter);
        }
        if (ltm->tm_sec != lastSecond)
        {
            tickSound();
            drawClockHand((appHeight - yCenter)*0.62, 6, butoaneSUS, ltm->tm_sec-1, 30, xCenter, yCenter);
            lastSecond = ltm->tm_sec;
        }

        drawClockHand((appHeight - yCenter)*0.35, 9, hotpink, ltm->tm_hour * 5, 30, xCenter, yCenter);
        drawClockHand((appHeight - yCenter)*0.48, 6, hotpink, ltm->tm_min, 30, xCenter, yCenter);
        drawClockHand((appHeight - yCenter)*0.61, 4, hotpink, ltm->tm_sec, 30, xCenter, yCenter);



        delay(100);

        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x_mouse = mousex();
            y_mouse = mousey();

            function_code = detect_button_and_function(0, x_mouse, y_mouse);
        }
        if ((function_code > -1) && (function_code < 5))
        {
            //closegraph();
            //cleardevice();
            return function_code;
        }
    }
}

//ok?

int main()
{
    int max_width = getmaxwidth(), max_height = getmaxheight();
    int app_width = max_width /2, app_height = max_height/2;

    app_height *= multiplicationFactor;
    app_width *= multiplicationFactor;

    int function_call_nr = 0;

    for (int i = 0; i < functions; i++)
    {
        buttons_array[i].left = -1;
        buttons_array[i].top = -1;
        buttons_array[i].right = -1;
        buttons_array[i].bottom = -1;
        buttons_array[i].function_code = -1;

        welcomePageButtonsArray[i].left = -1;
        welcomePageButtonsArray[i].top = -1;
        welcomePageButtonsArray[i].right = -1;
        welcomePageButtonsArray[i].bottom = -1;
        welcomePageButtonsArray[i].function_code = -1;
    }

    int currentWindowNr = initwindow(850, 620, "Project: CLOCK. Analog and Digital Clock.", (max_width - app_width)/2, 50,false, true);

    // int currentWindowNr = initwindow(app_width, app_height, "Clock. Analog and Digital Clock Project.", (max_width - app_width) / 2, (max_height - app_height) / 2, false, true);

    function_call_nr = welcome_page(max_width, max_height, currentWindowNr);

    while (TRUE)
    {
        setcurrentwindow(currentWindowNr);
        if ( function_call_nr == 0 )
        {
            function_call_nr = welcome_page(max_width, max_height, currentWindowNr);
        }
        if ( function_call_nr == 1 )
        {
            function_call_nr = analog_clock(max_width, max_height, currentWindowNr);
        }

        if ( function_call_nr == 4)
        {
            function_call_nr = alarm_menu(max_width, max_height);
        }
    }
    return 0;

}
