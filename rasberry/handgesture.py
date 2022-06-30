import time
from collections import Counter
import math
import csv
import cv2
import mediapipe as freedomtech
import os
import RPi.GPIO as gpio

def fonc1():
    buttonpin = 27
    buttonpout = 22

    gpio.setmode(gpio.BCM)
    gpio.setup(buttonpin,gpio.IN , pull_up_down=gpio.PUD_UP)
    gpio.setup(buttonpout,gpio.OUT)


    gpio.setmode(gpio.BCM)
    gpio.setup(12,gpio.OUT)
    gpio.setup(16,gpio.OUT)


    drawingModule = freedomtech.solutions.drawing_utils
    handsModule = freedomtech.solutions.hands
    mod=handsModule.Hands(max_num_hands=1)
    mod = handsModule.Hands(
            static_image_mode = False,
            max_num_hands=1,
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5)

    h=560
    w=1000
    # cap = cv2.VideoCapture("http://192.168.221.146:8080/video")
    cap = cv2.VideoCapture(0)
    tip=[8,12,16,20]
    tipname=[8,12,16,20]
    fingers=[]
    finger=[]
    listgest=[]


    def findpostion(frame1):
        list=[]
        results = mod.process(cv2.cvtColor(frame1, cv2.COLOR_BGR2RGB))
        if results.multi_hand_landmarks != None:
           for handLandmarks in results.multi_hand_landmarks:
               drawingModule.draw_landmarks(frame1, handLandmarks, handsModule.HAND_CONNECTIONS)
               list=[]
               for id, pt in enumerate (handLandmarks.landmark):
                    x = int(pt.x * w)
                    y = int(pt.y * h)
                    list.append([id,x,y])

        return list            

    def findnameoflandmark(frame1):
         list=[]
         results = mod.process(cv2.cvtColor(frame1, cv2.COLOR_BGR2RGB))
         if results.multi_hand_landmarks != None:
            for handLandmarks in results.multi_hand_landmarks:


                for point in handsModule.HandLandmark:
                     list.append(str(point).replace ("< ","").replace("HandLandmark.", "").replace("_"," ").replace("[]",""))
         return list


    def maybeMakeNumber(s):

        # éviter les problèmes avec les none , 0 , ou ""
        if not s:
            return s
        #convertit les str en int si c'est possible
        try:
            f = float(s)
            i = int(f)
            return i if f == i else f
        except ValueError:
            return s


    while True:
         ret, frame = cap.read()
         #retourne la camera 
         #reverse  = cv2.flip(frame, flipCode = -1)
         frame1 = cv2.resize(frame, (w, h))

         
         a=findpostion(frame1)
         b=findnameoflandmark(frame1)
       #   print(a)
       #   print(b)
         
         if len(b and a)!=0:
            finger=[]
            if a[0][1:] < a[4][1:]: 
               finger.append(1)
             #   print (b[4])
              
            else:
               finger.append(0)   

            
            
            
            fingers=[]
            listgest=[]
            for id in range(0,4):
                if a[tip[id]][2:] < a[tip[id]-2][2:]:
                   listgest.append(b[tipname[id]])
                   fingers.append(1)
        
                else:
                   fingers.append(0)
         x=fingers + finger
         c=Counter(x)
         up=c[1]
         down=c[0]


         listgest.append(up)
         listgest.append(down)
         
       # ['0', '5'] -- > 
       # ['INDEX FINGER TIP', 'PINKY TIP', '3', '2'] --> 
       # ['INDEX FINGER TIP', 'MIDDLE FINGER TIP', '3', '2'] -->
       # ['MIDDLE FINGER TIP', '1', '4'] -->
       # ['INDEX FINGER TIP', 'MIDDLE FINGER TIP', 'RING FINGER TIP', 'PINKY TIP', '5', '0'] -->


         if len(listgest) > 6:
            listgest = []
         
         gpio.output(12,gpio.LOW)
         gpio.output(16,gpio.LOW)
         with open('./model.csv',encoding='utf-8-sig') as f:
            keypoint_classifier_labels = csv.reader(f)
            for row in keypoint_classifier_labels:
               gesturecsv = row[:(len(row)-1)]
             # print(gesturecsv)
               #print(listgest)
               gest = row[(len(row)-1):]
             #   print(gest)
               gesturecsv = list(map(maybeMakeNumber, gesturecsv))
               if listgest == gesturecsv:
                  if gest == ['gauche']:
                      print("gauche")
                      gpio.output(12,gpio.HIGH)
                  if gest == ['droite']:
                      print("Droite")
                      gpio.output(16,gpio.HIGH)
    #               cv2.putText(frame1, "GESTE :"+str(gest), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 255), 2, cv2.LINE_AA)
    #       
    #      
#     #      cv2.imshow("Frame", frame1);
#           key = cv2.waitKey(1) & 0xFF
#     #      if key == ord("1"):
#     #         print("<-Learning Mode->")
#     #         if len(listgest)<=6:
#     #            gest = str(input("Name : "))
#     #            csv_path = './model.csv'
#     #            with open(csv_path,'a', encoding='utf-8', newline="") as f:
#     #               writer = csv.writer(f)
#     #               listgest.append(gest)
#     #               writer.writerow(listgest)
#     #         else: 
#     #            print("IMPOSSIBLE")
#           if key == ord("2"):
#               print("Bye")
#               break


