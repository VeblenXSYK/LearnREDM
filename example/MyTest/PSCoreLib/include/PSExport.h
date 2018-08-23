#pragma once
#include <string>

#ifndef _PSEXPORT_H_
#define _PSEXPORT_H_
#define PS_EXPORT_API  extern "C"  _declspec(dllexport)

#define FOREGROUND_NAME		"foreground"
#define BACKGROUND_NAME		"background"
#define CONTRAST_NAME		"contrast"
#define BALANCE_NAME		"balance"
#define SATURABILITY_NAME	"saturability"
#define AMBIENT_NAME		"ambient"
#define FIGURE_NAME			"figure"

enum ECUTOUTTYPE
{
	CT_LIGHT_BLUE,		// 浅蓝背景
	CT_DARK_BLUE,		// 深蓝背景
	CT_LIGHT_GREEN,		// 浅绿背景
	CT_DARK_GREEN,		// 深绿背景
};

// 异步回调函数声明,暂时不用
typedef void(CALLBACK *RecvImageCallback)(std::string imageData, int type);
typedef void(CALLBACK *RecvMessageCallback)(std::string msgData);

PS_EXPORT_API int PSSetImageCallback(void* core, RecvImageCallback callback);
PS_EXPORT_API int PSSetMessageCallback(void* core, RecvMessageCallback callback);

// 导出函数
PS_EXPORT_API void* PSInit();
PS_EXPORT_API void PSUninit(void* core);

PS_EXPORT_API int PSLogin(const void* core, std::string& server, std::string& password);
PS_EXPORT_API int PSLogout(const void* core);

/**
* @brief 发送自定义PS脚本到PhotoShop执行
* @param [in] core 来自PSInit()返回值
* @param [in] strCode 自定义PS脚本内容
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSSendJSCode(const void* core, std::string& strCode, std::string& outMsg);

/**
 * @brief 载入图片/PSD文件
 * @param [in] core 来自PSInit()返回值
 * @param [in] imgFile 图片/PSD文件路径
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSOpenFile(const void* core, std::string& imgFile, std::string& outMsg);

/**
* @brief 关闭当前激活的文档
* @param [in] core 来自PSInit()返回值
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSCloseActiveFile(const void* core, std::string& outMsg);

/**
* @brief 导出当前激活的文档到JPG图片
* @param [in] core 来自PSInit()返回值
* @param [in] strSaveFileName 导出JPG文件全路径
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSExportJPGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief 导出当前激活的文档到PNG图片
* @param [in] core 来自PSInit()返回值
* @param [in] strSaveFileName 导出PNG文件全路径
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSExportPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief 导出当前激活的文档的前景到PNG图片
* @param [in] core 来自PSInit()返回值
* @param [in] strSaveFileName 导出PNG文件全路径
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSExportForegroundPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief 导出当前激活的文档的背景到PNG图片
* @param [in] core 来自PSInit()返回值
* @param [in] strSaveFileName 导出PNG文件全路径
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSExportBackgroundPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief 导出当前激活的文档到PSD文件
* @param [in] core 来自PSInit()返回值
* @param [in] strSaveFileName 导出PSD文件全路径
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSExportPSDFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
 * @brief 获取当前激活文档截图
 * @param [in] core 来自PSInit()返回值
 * @param [in] width 指定获取图片宽度
 * @param [in] height 指定获取图片高度
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSGetDocumentImage(const void* core, int width, int height, std::string& outMsg);

 /**
 * @brief 获取载入PSD文件中前景截图
 * @param [in] core 来自PSInit()返回值
 * @param [in] width 指定获取图片宽度
 * @param [in] height 指定获取图片高度
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSGetForegroundLayerImage(const void* core, int width, int height, std::string& outMsg);

 /**
 * @brief 获取载入PSD文件中背景截图
 * @param [in] core 来自PSInit()返回值
 * @param [in] width 指定获取图片宽度
 * @param [in] height 指定获取图片高度
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSGetBackgroundLayerImage(const void* core, int width, int height, std::string& outMsg);

/**
 * @brief 解锁选择的图层并重新命名
 * @param [in] core 来自PSInit()返回值
 * @param [in] strNewLayerName 解锁后图层的名称
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSUnlockSelectLayer(const void* core, std::string strNewLayerName, std::string& outMsg);

/**
* @brief 删除指定名称的图层
* @param [in] core 来自PSInit()返回值
* @param [in] strLayerName 待删除图层的名称
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSDeleteSelectLayer(const void* core, std::string strLayerName, std::string& outMsg);

/**
 * @brief 对指定图层创建亮度/对比度图层
 * @param [in] core 来自PSInit()返回值
 * @param [in] strLayerName 需要创建亮度/对比度的图层名称
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSCreateContrastLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
 * @brief 设置亮度和对比度
 * @param [in] nBrigh 亮度，范围：-150 ~ 150
 * @param [in] nContr 对比度，范围：-50 ~ 100
 * @param [out] outMsg 函数执行完成后结果
 * @return 状态码 0-成功
 */
PS_EXPORT_API int PSSetContrastLayerByName(const void* core, int nBrigh, int nContr, std::string& outMsg);

/**
* @brief 对指定图层创建色彩平衡图层
* @param [in] core 来自PSInit()返回值
* @param [in] strLayerName 需要创建色彩平衡的图层名称
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSCreateColorBalanceLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
* @brief 设置色彩平衡
* @param [in] v1 暂不使用，默认：0
* @param [in] v2 色调值，范围：-100 ~ 100
* @param [in] v3 色温值，范围：-100 ~ 100
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSSetColorBalanceLayerByName(const void* core, int v1, int v2, int v3, std::string& outMsg);

/**
* @brief 对指定图层创建饱和度图层
* @param [in] core 来自PSInit()返回值
* @param [in] strLayerName 需要创建色彩平衡的图层名称
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSCreateSaturabilityLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
* @brief 设置饱和度
* @param [in] core 来自PSInit()返回值
* @param [in] nSatur 色温值，范围：-100 ~ 100
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSSetSaturabilityLayerByName(const void* core, int nSatur, std::string& outMsg);

/**
* @brief 设置环境色
* @param [in] core 来自PSInit()返回值
* @param [in] strLayerName 需要设置环境色的图层名称
* @param [in] fRed 红色，范围：0 ~ 255
* @param [in] fGreen 绿色，范围：0 ~ 255
* @param [in] fBlue 蓝色，范围：0 ~ 255
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSSetAmbientColor(const void* core, std::string strLayerName, float fRed, float fGreen, float fBlue, std::string& outMsg);

/**
* @brief 对指定图片进行抠图操作
* @param [in] core 来自PSInit()返回值
* @param [in] strLayerName 需要抠图的图层名称
* @param [in] eCutoutType 被抠图图片的背景颜色类型
* @param [out] outMsg 函数执行完成后结果
* @return 状态码 0-成功
*/
PS_EXPORT_API int PSCutoutByName(const void* core, std::string strLayerName, ECUTOUTTYPE eCutoutType, std::string& outMsg);

#endif