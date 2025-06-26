import sensor
import time
import image
from pyb import UART
import math


###########################################
goal_yellow =(39, 86, -38, -11, 45, 127)# ゴールの色取り用変数(黄色)##A

##goal_yellow =(57, 91, -42, 3, 99, 17)##B

##goal_yellow =(97, 62, -40, 64, 18, 73)
#######################################################
court_green =(50, 77, -26, 2, -12, 28)#a
#court_green =(57, 85, -86, -37, -128, 127)#b
# コートの色取り用変数A
##court_green =(86, 46, -39, 33, -31, 12)# コートの色取り用変数B

##court_green =(94, 51, -43, -90, -14, 80)# 調整用コート


court_light = (90, 62, -69, -15, -25, 85)
#############################################################
goal_blue = (5, 50, 43, 0, -59, -22)##A

##blue = (23, 57, 17, -28, -53, -16)##B

#############################################################

black = (0, 36, -43, 41, -19, 12)

wall_balck = [0,0]

screen_center = [137, 127]                  # 画面の中央座標





#########################################################
yellow = [0, 0]
count_goal_yellow      =   0

yellow_num = 0

dir_goal_yellow              =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_goal_yellow         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列

###############################################################
court = [0, 0]
court_dir = 0
count_court_green      =   0

court_green_num = 0

dir_court_green             =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_court_green         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列

###############################################################
blue = [0, 0]
count_goal_blue     =   0

blue_num = 0

dir_goal_blue              =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_goal_blue         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列

###############################################################






sensor.reset()  # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)  # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)  # Set frame size to QVGA (320x240)
sensor.skip_frames(time=2000)  # Wait for settings take effect.
clock = time.clock()  # Create a clock object to track the FPS.
uart = UART(3, 115200, timeout_char=1000)
while True:
    clock.tick()  # Update the FPS clock.
    img = sensor.snapshot()  # Take a picture and return the image.
    # to the IDE. The FPS should increase once disconnected.
    img.draw_cross(screen_center[0], screen_center[1])
    img.draw_circle(screen_center[0], screen_center[1],270,[0,0,0],180)
    img.draw_circle(screen_center[0], screen_center[1],70,[0, 0, 0], 1, 1)
        #img.draw_circle(screen_center[10], screen_center[1], 12,[0,0,0],50
    # img.draw_rectangle(0,0,20,300,[0,0,0],1,1)
    # img.draw_rectangle(260,0,100,300,[0,0,0],1,1)



    yellow_dir = 300;
    blue_dir = 300;


    for blob in img.find_blobs([goal_blue], pixels_threshold=10, area_threshold=10, merge=True,margin=25):
        img.draw_rectangle(blob.rect())
        if count_goal_yellow >= 2:
           count_goal_yellow = 0;

        yellow[0] = blob.cx()
        yellow[1] = blob.cy()

        img.draw_line(screen_center[0], screen_center[1], yellow[0], yellow[1], thickness=2)

        yellow_dir = ((math.atan2(yellow[0] - 150 , yellow[1] - 105 ) * 180 / 3.14 + 180) - 90 + 360) % 360 - 90;

        dir_goal_yellow[count_goal_yellow] = yellow_dir;
        area_goal_yellow[count_goal_yellow] = blob.area();

        yellow_num = area_goal_yellow.index(max(area_goal_yellow[:]));

        yellow_dir = dir_goal_yellow[yellow_num];
        yellow_dir = yellow_dir - 90;
        # if yellow_dir > 180:
        #     yellow_dir = yellow_dir - 360;

        count_goal_yellow += 1;

        #yellow_dir = yellow_dir + 180;

        yellow_dir = yellow_dir * -2;


#    for blob in img.find_blobs([goal_blue], pixels_threshold=10, area_threshold=10, merge=True,margin=25):
#        img.draw_rectangle(blob.rect())
#        if count_goal_blue >= 2:
#            count_goal_blue = 0;

#        blue[0] = blob.cx()
#        blue[1] = blob.cy()

#        img.draw_line(screen_center[0], screen_center[1], blue[0], blue[1],color=(0,255,0), thickness=2)

#        blue_dir = ((math.atan2(blue[0] - 150 , blue[1] - 105 ) * 180 / 3.14 + 180) - 90 + 360) % 360 - 90;

#        dir_goal_blue[count_goal_blue] = blue_dir;
#        area_goal_blue[count_goal_blue] = blob.area();

#        blue_num = area_goal_blue.index(max(area_goal_blue[:]));

#        blue_dir = dir_goal_blue[blue_num];

#        if blue_dir > 180:
#            blue_dir = blue_dir - 360;

#        count_goal_blue += 1;

#        #yellow_dir = yellow_dir + 180;

#        blue_dir = blue_dir * -2;




    for blob in img.find_blobs([court_green], pixels_threshold=50, area_threshold=50, merge=True,margin=25):
        img.draw_rectangle(blob.rect())


        if count_court_green >= 2:
           count_court_green = 0;

        court[0] = blob.cx()
        court[1] = blob.cy()

        img.draw_line(screen_center[0], screen_center[1], court[0], court[1],color=(255,0,0), thickness=2) # 画面中心からコート中心へのライン描画

        court_dir = ((math.atan2(court[0] - 150 , court[1] - 105 ) * 180 / 3.14 + 180) - 90 + 360) % 360;

        court_dir = court_dir - 180;
        # print(court_dir)
        # print("  ")
        if court_dir > 157.5:
            court_dir = 180;

        elif court_dir > 112.5:
            court_dir = 135;

        elif court_dir  > 67.5:
           court_dir = 90;

        elif court_dir  > 22.5:
            court_dir = 45;

        elif court_dir  > -22.5:
           court_dir = 0;
        elif court_dir  > -67.5:
           court_dir = -45;

        elif court_dir  > -112.5:
           court_dir = -90;

        elif court_dir  > -157.5:
            court_dir = -135;

        else:
            court_dir = -180;




        dir_court_green[count_court_green] = court_dir;
        area_court_green[count_court_green] = blob.area();

        court_green_num = area_court_green.index(max(area_court_green[:]));

        court_dir = dir_court_green[court_green_num];


        # if court_dir < 0:
            # court_dir = court_dir + 360;


        count_court_green += 1;

    #########################################



    #print(court_dir)
    # print("  ")
    #print(court[0])
    #print(court[1])
    #print("  ")
    #print(yellow_dir)
    #print("\n")


    uart.write(str(court_dir))
    uart.write("a")
    uart.write(str(yellow_dir))
    uart.write("b")
#    uart.write(str(blue_dir))
#    uart.write("c")
