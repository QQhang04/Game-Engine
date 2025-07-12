# // todo

1.sort all the entities of our system by z-index before rendering性能太差

采用什么排序策略以及什么时机进行排序需要根据游戏类型来进行优化



2.删除entity的component之后没有及时更新system中的entity情况

3.临时给entity加上component之后能否顺利被系统识别

4.component的enable，disable ；entity的enable，disable

5.优化addEntityToSystems，而不是每一次都循环遍历所有的system



6.EventSystem里做到高级事件系统, 如分层，tunnel……