# bing-bg
解析xml，获取每日bing背景图片并设置为壁纸

# 技术要点：
使用Windows API
- 创建删除文件/文件夹：CreateDirectory DeleteFile
- 系统设置：SystemParametersInfoA
- 下载文件：URLDownloadToFileA

使用了tinyxml开源库解析xml


# 环境
Windows SDK版本：10.0.17134.0

平台：Visual Studio 2017 (v141)

# 去重
## 均值哈希 average_hash
average_hash是perceptual hash算法的一种。

过程：
1. 图像缩放—将图像缩放到N*N大小
2. 灰度化—对N*N大小的图像进行灰度化
3. 计算均值—计算这N*N大小图片中所有像素的均值
4. 得到N*N图像的ahash—N*N的像素值中大于均值的则用1表示，小于的用0表示，这样就得到一个N*N位二进制码作为该图像的ahash值。
5. 计算两幅图像ahash值的汉明距离，距离越小，表明两幅图像越相似；距离越大，表明两幅图像距离越大。

程序中我用了8*8，阈值为5，如果觉得精度不高可以增加为16*16阈值为3等等


__优点__
简单，计算快

__缺点__
敏感性较高，图片稍微变化对结果影响极大

头文件dirent.h使用了JackSoja写的兼容windows的文件 https://github.com/JackSoja/dirent
> 参考：https://blog.csdn.net/u011402197/article/details/48677813


## 感知哈希 Perceptual_hashing
DCT（离散余弦变换）具有很强的”能量集中”特性，左上方称为低频数据，右下方称为高频数据。而大多数的自然信号(包括声音和图像)的能量都集中在离散余弦变换后的低频部分。因此也可以在图像压缩算法中用来进行有损压缩。（如JPEG压缩编码）
根据此原理取左上角做均值哈希。
过程：
1. 将图像转为灰度图
2. 将图片大小调整为32*32像素
3. 通过DCT变换，再取左上角的8*8像素区域（低频部分）
4. 计算该区域灰度均值，获取图像指纹
5. 计算两幅图像ahash值的汉明距离

> 参考：https://blog.csdn.net/akadiao/article/details/79778095
> https://blog.csdn.net/eternity1118_/article/details/51088019
> DCT: https://blog.csdn.net/yanceyxin/article/details/82080242

# To do list
- <del>正则将url里分辨率匹成1920*1080</del>
- 增加下载以前的图片的功能
- 随机更换
- <del>去重</del>
- 去重测试


# others
关于url参数 "http://cn.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1"
- format，非必要。返回结果的格式，不存在或者等于xml时，输出为xml格式，等于js时，输出json格式。
- idx，非必要。不存在或者等于0时，输出当天的图片，-1为已经预备用于明天显示的信息，1则为昨天的图片，idx最多获取到前7天的图片信息。
- n，必要参数。这是输出信息的数量。比如n=1，即为1条，以此类推，至多输出8条。