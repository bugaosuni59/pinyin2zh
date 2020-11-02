# 源代码及用法：



(1) trainer.cpp：使用原始数据集(GBK编码)进行训练，得到一元、二元、三元字模型的训练结果，分别保存在data1.txt, data2.txt, data3.txt中

(2) fenci.py：对原始数据集分词，并保存为分词数据

(3) fencitrainer.cpp：对分词数据进行训练，得到一元、二元词模型的训练结果，分别保存在dataci1.txt, dataci2.txt中

(4) app.cpp：使用训练好的一元、二元、三元字模型，一元、二元词模型，完成拼音转文字功能。具体用法为：app.exe input.txt output.txt

(5) checker.cpp：用于计算正确率，检查两个输出文件相同汉字的数量和百分比。具体用法为：checker.exe output1.txt output2.txt

trainer.cpp执行约1小时

fenci.py执行约4小时

fencitrainer.cpp执行约10分钟

app.cpp在二元字模型下速度较快，半分钟内完成计算，但在三元字、二元词模型下较慢，根据输入规模需要计算2分钟以上。

