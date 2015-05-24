#ifndef __Plist_Tool_H__
#define __Plist_Tool_H__

#include <string>
#include <vector>

#include "cocos2d.h"


typedef std::function<void()> pFuncUnpackFinishedCallback;

class PlistTool
{
public:
	PlistTool();
	~PlistTool();

	void addUnpackList(const std::vector<std::string>& list);
	void startUnpack(pFuncUnpackFinishedCallback func);



protected:
	void updateCheckEnd(float dt);

	// step 0.
	bool isLastPlistProcessFinish();

	// step 1.
	void setSavePath(const char* savePath);

	// step 2.
	void unpackTextureByPlist(const char* plistFile);

	void timeToSaveFile(float dt);

	// one frame save finished
	void onFrameSaveFinished(cocos2d::RenderTexture*, const std::string&);

	std::string generateFileName(const char* plistName);

private:
	pFuncUnpackFinishedCallback m_unpackFinishedCallfunc;
	std::vector<std::string> m_willUnpackList;

	bool m_bProcessFinished;

	int m_iFramesCount;
	std::string m_savePath;
	cocos2d::Map<std::string, cocos2d::RenderTexture*> m_textureList;
};


#endif // __Plist_Tool_H__
