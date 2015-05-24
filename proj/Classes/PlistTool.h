#ifndef __Plist_Tool_H__
#define __Plist_Tool_H__

#include <string>

#include "cocos2d.h"

class PlistTool
{
public:
	PlistTool();
	~PlistTool();

	// step 0.
	bool isLastPlistProcessFinish();

	// step 1.
	void setSavePath(const char* savePath);

	// step 2.
	void unpackTextureByPlist(const char* plistFile);


protected:
	void timeToSaveFile(float dt);
	// one frame save finished
	void onFrameSaveFinished(cocos2d::RenderTexture*, const std::string&);

	std::string generateFileName(const char* plistName);

private:
	bool m_bProcessFinished;

	int m_iFramesCount;
	std::string m_savePath;
	cocos2d::Map<std::string, cocos2d::RenderTexture*> m_textureList;
};


#endif // __Plist_Tool_H__
