# Bluetooth_SPP_mechanum_controller
UARTから来る信号により、メカナムホイールを動作させる物。  
mechanummoverオブジェクト生成時に与えられている引数を変更することで、   
それぞれのピン番号を変更することができる。  

データフォーマット   
{-1.0~1.0の少数｝,{xかｙかz};   
example   
0.1,x;   
{これでゆっくり横移動｝   
0.0,x;   
{横移動停止｝   
0.4,x;   
0.4,y;   
{斜め移動｝   
0.0,x;   
0.0,y;   
{斜め移動停止｝   
0.2,z;   
{右旋回｝   
0.0,z;   
{旋回停止｝   

   
動作検証機材   

|Genre|NAME|
|:--:|:--:|
|Arduino|Arduino  NANO(328p)|
|Bluetooth module|SBDBT5V|
|MOTORDRIVER|MD10C|

[製作者　ゆきまくら（twitter)](https://twitter.com/yukimakura86)
