# fixed-wingDIY
采用stm32f103c8t6主控，2.4gNRF24L01通信，遥控控制姿态，3个9g舵机，80cm翼展，HLK-DL04A和2204+8060桨叶，2SGNB电池

全机重260g，机翼翼载21.67g/dm^2，采用kt热板和轻木条固定
没有方向舵，通过角度差调控副翼实现减小转弯副翼带来不同阻力下会导致侧滑影响
全机长60cm，垂直尾翼8*18*4，水平尾翼30*6，升降舵在尾翼上切割，2*30，机翼采用弯折法平凸起翼型
<img width="3072" height="4096" alt="IMG_20261011_173859" src="https://github.com/user-attachments/assets/6685938b-e94b-4376-a8e6-0d1a4ac1fe8e" />
<img width="4096" height="3072" alt="IMG_20261010_132451" src="https://github.com/user-attachments/assets/51123a21-0f8b-4df4-a5c2-79807b1a0fc9" />
<img width="3072" height="4096" alt="IMG_20261011_211728" src="https://github.com/user-attachments/assets/361afee3-190d-4661-a634-e492cc30183f" />
<img width="4096" height="3072" alt="IMG_20261012_224112" src="https://github.com/user-attachments/assets/44c8f401-0d0c-46bf-b2d7-55dcf38a0a52" />
这是立创原理图飞控部分，MPU6050为陀螺仪预留接口，还有串口预留接口，还有1组TIMPWM预留，目前还没有实现自稳，可以自己选择编辑，有开关电源，将电机的电流没有通过pmos，防止烧mos管
<img width="472" height="404" alt="屏幕截图 2026-10-22 213423" src="https://github.com/user-attachments/assets/1a5448eb-8d7e-4eff-94fa-f7eace7e13b6" />
<img width="783" height="491" alt="屏幕截图 2026-10-22 213438" src="https://github.com/user-attachments/assets/49a6f835-da0a-46a6-a6a8-d8503da28ae6" />

这有飞行效果


https://github.com/user-attachments/assets/369f4b73-922c-42b4-8883-f78a17a1906d

