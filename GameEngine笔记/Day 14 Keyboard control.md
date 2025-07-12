## Day 14 Keyboard control

通过键盘控制上下左右使得被KeyboardControlSystem控制（**即拥有**`CharacterControlledComponent`）的entity能够上下左右移动

<img src="/Users/qqhang/Library/Application Support/typora-user-images/image-20250706131900604.png" alt="image-20250706131900604" style="zoom:50%;" />

###### chopper-sprite-sheet

col：animation中的不同帧

row：不同的状态



##### 整体思路:

1. game的handleinput中发起按键按下以及释放的事件

2. 在`CharacterControlledComponent`中接收事件，并且根据此计算出最终的方向`finalDir`

3. finalDir和CharacterControlledComponent中的speed相乘来改变`Rigidbodycomponent`中的velocity

   finalDir来改变`SpriteComponent`中的srcRect的y值