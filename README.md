# Qt-ICQ
项目描述：个人练习项目，QT实现多个客户端进行通讯

使用方法：先启动服务器，后启动客户端，可以启动多个客户端。客户端之间可以在公屏进行信息通讯，类似群聊。也可以两个客户端点对点的通讯，类似好友私聊。

涉及技术：QT基础控件，QT槽函数，QTableWidget表格控件使用，QFile读写文件，Qtxml对xml文件的增删改查，利用栈stack实现算术表达式的计算

文件描述：

         widget，主界面ui，按钮的槽函数
         
         calculator_method，实现计算器的计算功能
         
         history_method，实现历史记录的存储功能，用xml文件存储
         
         history_widget，历史记录ui，读取xml文件的历史记录

![image]([public/image/calculatorWidget.png](https://github.com/Eternal-zdh/QTcalculator/blob/main/image/calculatorWidget.png))
![image]([public/image/historyWidget.png](https://github.com/Eternal-zdh/QTcalculator/blob/main/image/historyWidget.png))
