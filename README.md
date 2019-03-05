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


# To do list
- 正则将url里分辨率匹成1920*1080
- 正则匹出url
- 增加下载以前的图片的功能
- 随机更换
- 去重
- 删除
- 打个包

# else
关于url参数 "http://cn.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1"
format，非必要。返回结果的格式，不存在或者等于xml时，输出为xml格式，等于js时，输出json格式。
idx，非必要。不存在或者等于0时，输出当天的图片，-1为已经预备用于明天显示的信息，1则为昨天的图片，idx最多获取到前7天的图片信息。
n，必要参数。这是输出信息的数量。比如n=1，即为1条，以此类推，至多输出8条。