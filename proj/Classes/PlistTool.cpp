#include "PlistTool.h"
#include <vector>

#include "cocos2d.h"
using namespace cocos2d;

//长款小于等于该值的图片直接舍去
#define INVALID_IMAGE_WIDTH 2
#define INVALID_IMAGE_HEIGHT 2


PlistTool::PlistTool()
	: m_bProcessFinished(true)
{

}
PlistTool::~PlistTool()
{
	m_textureList.clear();
}

bool PlistTool::isLastPlistProcessFinish()
{
	return m_bProcessFinished;
}

// step 1.
void PlistTool::setSavePath(const char* savePath)
{
	if (!m_bProcessFinished)
	{
		MessageBox("last plist file did not process end", "Error");
		return;
	}

	m_savePath = savePath;
}

// step 2.
void PlistTool::unpackTextureByPlist(const char* plistFile)
{
	if (!m_bProcessFinished)
	{
		MessageBox("last plist file did not process end", "Error");
		return;
	}

	m_bProcessFinished = false;
	char tempbuf[1024] = { 0 };

	//0. clear
	m_textureList.clear();
	SpriteFrameCache* pSFC = SpriteFrameCache::getInstance();
	pSFC->removeSpriteFrames();
	pSFC->addSpriteFramesWithFile(plistFile);

	//generate file pre-name
	std::string directoryPath;
	::GetCurrentDirectoryA(sizeof(tempbuf), tempbuf);
	std::string preName = generateFileName(plistFile);
	::CreateDirectoryA((tempbuf + std::string("/") + preName).c_str(), NULL);
	directoryPath = tempbuf + std::string("/") + preName + "/";


	//1. get texture data
	Map<std::string, SpriteFrame*> framesMap = pSFC->getSpriteframes();
	//sort
	for (Map<std::string, SpriteFrame*>::iterator it1 = framesMap.begin(); it1 != framesMap.end(); ++it1)
	{
		for (Map<std::string, SpriteFrame*>::iterator it2 = it1; it2 != framesMap.end(); ++it2)
		{
			if (strcmp(it1->first.c_str(), it2->first.c_str()) > 0)
			{
				std::string tempFirst = it1->first;
				SpriteFrame* tempSecond = it1->second;
				std::string* pFirst = const_cast<std::string*>(&it1->first);
				*pFirst = it2->first;
				it1->second = it2->second;
				pFirst = const_cast<std::string*>(&it2->first);
				*pFirst = tempFirst;
				it2->second = tempSecond;
			}
		}
	}


	//2. add render command
	//m_iFramesCount = framesMap.size();  //delete invalid image
	m_iFramesCount = 0;
	char fileKeyBuf[1024] = { 0 };
	for (Map<std::string, SpriteFrame*>::const_iterator itor = framesMap.begin(); itor != framesMap.end(); ++itor)
	{
		SpriteFrame* frame = itor->second;
		GLuint textName = frame->getTexture()->getName();
		const Rect& rect = frame->getRectInPixels();
		bool isRotate = frame->isRotated();
		const Vec2& offset = frame->getOffsetInPixels();
		const Size& origSize = frame->getOriginalSizeInPixels();

		// 去掉 过小的无效图片 （加密后?的plist会生成很多无效图片）
		if (rect.size.width <= INVALID_IMAGE_WIDTH && rect.size.height <= INVALID_IMAGE_HEIGHT)
		{
			continue;
		}

		// key --- 去掉重复的图片 （加密后?的plist会有很多张重复图片）
		sprintf(fileKeyBuf, "%d_%.1f%.1f%.1f%.1f_%s_%.1f%.1f_%.1f%.1f",
			textName, 
			rect.origin.x, rect.origin.y, rect.size.width, rect.size.height,
			isRotate ? "1" : "0",
			offset.x, offset.y,
			origSize.width, origSize.height);
		if (m_textureList.find(fileKeyBuf) != m_textureList.end())
		{
			continue;
		}

		sprintf(tempbuf, "%0004d.png", m_iFramesCount);
		Sprite* pSp = Sprite::createWithSpriteFrame(itor->second);
		RenderTexture* texture = RenderTexture::create(pSp->getContentSize().width, pSp->getContentSize().height);
		texture->setName(directoryPath + tempbuf);
		//texture->setName(itor->first + ".png");
		texture->begin();
		pSp->setPosition(pSp->getContentSize()/2); //--- be careful
		pSp->visit();
		texture->end();
		m_textureList.insert(fileKeyBuf, texture);
		++m_iFramesCount;
	}
	
	//3. set render end callback
	Director::getInstance()->getScheduler()->schedule(std::bind(&PlistTool::timeToSaveFile, this, std::placeholders::_1), 
		Director::getInstance()->getRunningScene(), 0, 0, false, false, "PlistTool_TimeToSaveFile");
}

void PlistTool::timeToSaveFile(float dt)
{
	std::vector<std::string> saveSucVec;
	std::vector<std::string> saveFailVec;
	bool ret;

	for (Map<std::string, RenderTexture*>::const_iterator itor = m_textureList.begin(); itor != m_textureList.end(); ++itor)
	{
		Image* image = itor->second->newImage(true);
		if (image)
		{
			image->saveToFile(itor->second->getName().c_str(), false);
		}
		CC_SAFE_DELETE(image);

		onFrameSaveFinished(itor->second, itor->second->getName());
	}
}

void PlistTool::onFrameSaveFinished(cocos2d::RenderTexture*, const std::string& filename)
{
	CCLOG("file %s save finish", filename.c_str());
	--m_iFramesCount;
	if (m_iFramesCount <= 0)
	{
		m_bProcessFinished = true;
	}
}




std::string PlistTool::generateFileName(const char* plistName)
{
	std::string file(plistName, strlen(plistName) - strlen(".plist"));
	std::string::size_type pos = file.find_last_of("/");
	if (pos != std::string::npos)
	{
		file.substr(pos);
	}

	while (file.length() > 0)
	{
		char lastChar = file.at(file.length() - 1);
		if (lastChar >= '0' && lastChar <= '9')
		{
			file = file.substr(0, file.length() - 2);
		}
		else
		{
			break;
		}
	}

	return file.empty() ? plistName : file;
}
