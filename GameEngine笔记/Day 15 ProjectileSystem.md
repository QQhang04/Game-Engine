## Day 15 ProjectileSystem

##### ProjectileEmiterComponent

加在要发射导弹的entity上，有记录上一次发射时间、发生频率等数据



##### ProjectileComponent

加在要发射的导弹上，记录伤害、生命、创建时间等数据



##### ProjectileEmitSystem

在Game的Update中传入Registry引用，根据时间是否满足动态创建导弹entity，并为其添加component，比如ProjectileComponent



##### ProjectileLifecycleSystem

在Update中管理所有导弹，一旦超时，直接remove（也同上使用Registry）