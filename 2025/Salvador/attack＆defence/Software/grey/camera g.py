import sensor
import time
import image
from pyb import UART
import math

###########################################
goal_yellow =(58, 96, -33, 6, 27, 58)# ゴールの色取り用変数(黄色)##A

#######################################################
#コート
#court_green =(62, 93, -19, 34, -28, 11)
# カーペット
court_green =(44, 88, -58, -34, -74, 20)



court_light = (30, 88, -71, -27, -40, 18)
#############################################################


goal_blue =(35, 73, -10, 72, -58, -32)##A
#############################################################

black = (0, 36, -43, 41, -19, 12)

wall_balck = [0,0]

screen_center = [150, 110]                  # 画面の中央座標

court = [0, 0]

yellow = [0, 0]

blue = [0,0]

court_dir = 0
#########################################################
count_goal_blue      =   0

blue_num = 0

dir_goal_blue              =   [0, 0]    # 黄ゴールの中心x座標保存用配列
dis_goal_blue             =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_goal_blue         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列



#########################################################
count_goal_yellow      =   0

yellow_num = 0

dir_goal_yellow              =   [0, 0]    # 黄ゴールの中心x座標保存用配列
dis_goal_yellow              =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_goal_yellow         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列

###############################################################
count_court_green      =   0

court_green_num = 0

dir_court_green             =   [0, 0]    # 黄ゴールの中心x座標保存用配列
area_court_green         =   [0, 0]   # 黄ゴールの色取りエリア保存用配列

###############################################################



sensor.set_saturation(0)##彩度
sensor.set_brightness(0)##明度
sensor.set_contrast(0)#明暗sensor.set_saturation(0);


sensor.set_auto_gain(False);
sensor.set_auto_exposure(False);


sensor.reset()  # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)  # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)  # Set frame size to QVGA (320x240)
sensor.skip_frames(time=2000)  # Wait for settings take effect.
clock = time.clock()  # Create a clock object to track the FPS.
uart = UART(3, 115200, timeout_char=1000)
while True:
    clock.tick()  # Update the FPS clock.
    img = sensor.snapshot()##.gamma_corr(gamma = 1.3, contrast = 1.233, brightness = 0.1)
    # to the IDE. The FPS should increase once disconnected.
    img.draw_cross(screen_center[0], screen_center[1])

    img.draw_circle(screen_center[0], screen_center[1],180,[0,0,0],90)

    img.draw_circle(screen_center[0], screen_center[1],57,[0, 0, 0], 1, 1)
        #img.draw_circle(screen_center[10], screen_center[1], 12,[0,0,0],50)



    yellow_dir = 500;
    court_dir = 500;
    yellow_dis = 500;
    blue_dir = 500;
    blue_dis = 500;


    for blob in img.find_blobs([goal_blue], pixels_threshold=10, area_threshold=10, merge=True,margin=25):
        img.draw_rectangle(blob.rect())
        if count_goal_blue >= 2:
           count_goal_blue = 0;

        blue[0] = blob.cx()
        blue[1] = blob.cy()
        blue_dis = math.sqrt(math.pow(blue[0] - screen_center[0] ,2) + math.pow(blue[1] -screen_center[1],2));

        img.draw_line(screen_center[0], screen_center[1], blue[0], blue[1],color=(0,32,255), thickness=2)

        blue_dir = ((math.atan2(blue[0] - screen_center[0] , blue[1] - screen_center[1] ) * 180 / 3.14 + 180) - 90 + 360) % 360 - 90;

        dir_goal_blue[count_goal_blue] = blue_dir;
        dis_goal_blue[count_goal_blue] = blue_dis;
        area_goal_blue[count_goal_blue] = blob.area();

        blue_num = area_goal_blue.index(max(area_goal_blue[:]));

        blue_dir = dir_goal_blue[blue_num];
        blue_dis = dis_goal_blue[blue_num];

        if blue_dir > 180:
            blue_dir = blue_dir - 360;

        count_goal_blue += 1;

        #yellow_dir = yellow_dir + 180;

#        yellow_dir = yellow_dir;


    for blob in img.find_blobs([goal_yellow], pixels_threshold=10, area_threshold=10, merge=True,margin=25):
        img.draw_rectangle(blob.rect())
        if count_goal_yellow >= 2:
            count_goal_yellow = 0;

        yellow[0] = blob.cx()
        yellow[1] = blob.cy()
        yellow_dis = math.sqrt(math.pow(yellow[0] - screen_center[0] ,2) + math.pow(yellow[1] -screen_center[1],2));

        img.draw_line(screen_center[0], screen_center[1], yellow[0], yellow[1],color=(255,255,0), thickness=2)

        yellow_dir = ((math.atan2(yellow[0] - screen_center[0] , yellow[1] - screen_center[1] ) * 180 / 3.14 + 180) - 90 + 360) % 360 - 90;

        dir_goal_yellow[count_goal_yellow] = yellow_dir;
        dis_goal_yellow[count_goal_yellow] = yellow_dis;
        area_goal_yellow[count_goal_yellow] = blob.area();

        yellow_num = area_goal_yellow.index(max(area_goal_yellow[:]));

        yellow_dir = dir_goal_yellow[yellow_num];
        yellow_dis = dis_goal_yellow[yellow_num];

        if yellow_dir > 180:
            yellow_dir = yellow_dir - 360;

        count_goal_yellow += 1;





    for blob in img.find_blobs([court_green], pixels_threshold=50, area_threshold=50, merge=True,margin=25):
        img.draw_rectangle(blob.rect())


        if count_court_green >= 2:
           count_court_green = 0;

        court[0] = blob.cx()
        court[1] = blob.cy()

        img.draw_line(screen_center[0], screen_center[1], court[0], court[1],color=(0,190,0), thickness=2) # 画面中心からコート中心へのライン描画



        court_dir = ((math.atan2(court[0] - screen_center[0] , court[1] - screen_center[1] ) * 180 / 3.14 + 180) - 90 + 360) % 360;

        dir_court_green[count_court_green] = court_dir;
        area_court_green[count_court_green] = blob.area();

        court_green_num = area_court_green.index(max(area_court_green[:]));

        court_dir = dir_court_green[court_green_num];

        court_dir = court_dir - 90;

        if court_dir < 0:
            court_dir = court_dir + 360;


        count_court_green += 1;

    #########################################



#    print(court_dir)
#    print("  ")
#    print(yellow_dir)
#    print(yellow_dis)
#    print("\n")


    uart.write(str(court_dir))
    uart.write("      ")
    uart.write("a")
    uart.write(str(yellow_dir))
    uart.write("      ")
    uart.write("b")
    uart.write(str(yellow_dis))
    uart.write("      ")
    uart.write("c")
    uart.write(str(blue_dir))
    uart.write("      ")
    uart.write("d")
    uart.write(str(blue_dis))
    uart.write("      ")
    uart.write("e")
    uart.flush()
