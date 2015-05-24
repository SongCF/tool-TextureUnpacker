# PlistTextureUnpacker
将TexturePacker打包的纹理大图(plist+图片)还原为每一张小图

#说明
使用cocos2d-x引擎实现
- proj目录下为项目文件
- win32.exe目录下为编译好的可执行文件（win7）

#如何使用
1. 首先新建一个cocos2d-x工程（使用的3.x版本）
2. 将2dx_file目录下的文件用于替换原cocos2dx引擎的文件（SpriteFrameCache只是多加了一个函数priteFrameCache::getSpriteframes()）
3. 替换Classes 和 Resource
4. 编译运行即可