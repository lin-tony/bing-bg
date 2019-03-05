# bing-bg
每日获取bing背景图片并设置为壁纸

# 技术要点：
使用Windows API
- 创建删除文件/文件夹：CreateDirectory DeleteFile
- 系统设置：SystemParametersInfoA
- 下载文件：URLDownloadToFileA

使用了tinyxml开源库解析xml

# 环境
Windows SDK版本: 10.0.17134.0
平台: Visual Studio 2017 (v141)

# To do list
- 正则将url里分辨率匹成1920*1080
- 正则匹出url
- 增加下载以前的图片的功能
- 随机更换
- 去重
- 删除
- 打个包
