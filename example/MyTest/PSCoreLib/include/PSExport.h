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
	CT_LIGHT_BLUE,		// ǳ������
	CT_DARK_BLUE,		// ��������
	CT_LIGHT_GREEN,		// ǳ�̱���
	CT_DARK_GREEN,		// ���̱���
};

// �첽�ص���������,��ʱ����
typedef void(CALLBACK *RecvImageCallback)(std::string imageData, int type);
typedef void(CALLBACK *RecvMessageCallback)(std::string msgData);

PS_EXPORT_API int PSSetImageCallback(void* core, RecvImageCallback callback);
PS_EXPORT_API int PSSetMessageCallback(void* core, RecvMessageCallback callback);

// ��������
PS_EXPORT_API void* PSInit();
PS_EXPORT_API void PSUninit(void* core);

PS_EXPORT_API int PSLogin(const void* core, std::string& server, std::string& password);
PS_EXPORT_API int PSLogout(const void* core);

/**
* @brief �����Զ���PS�ű���PhotoShopִ��
* @param [in] core ����PSInit()����ֵ
* @param [in] strCode �Զ���PS�ű�����
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSSendJSCode(const void* core, std::string& strCode, std::string& outMsg);

/**
 * @brief ����ͼƬ/PSD�ļ�
 * @param [in] core ����PSInit()����ֵ
 * @param [in] imgFile ͼƬ/PSD�ļ�·��
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSOpenFile(const void* core, std::string& imgFile, std::string& outMsg);

/**
* @brief �رյ�ǰ������ĵ�
* @param [in] core ����PSInit()����ֵ
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSCloseActiveFile(const void* core, std::string& outMsg);

/**
* @brief ������ǰ������ĵ���JPGͼƬ
* @param [in] core ����PSInit()����ֵ
* @param [in] strSaveFileName ����JPG�ļ�ȫ·��
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSExportJPGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief ������ǰ������ĵ���PNGͼƬ
* @param [in] core ����PSInit()����ֵ
* @param [in] strSaveFileName ����PNG�ļ�ȫ·��
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSExportPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief ������ǰ������ĵ���ǰ����PNGͼƬ
* @param [in] core ����PSInit()����ֵ
* @param [in] strSaveFileName ����PNG�ļ�ȫ·��
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSExportForegroundPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief ������ǰ������ĵ��ı�����PNGͼƬ
* @param [in] core ����PSInit()����ֵ
* @param [in] strSaveFileName ����PNG�ļ�ȫ·��
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSExportBackgroundPNGFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
* @brief ������ǰ������ĵ���PSD�ļ�
* @param [in] core ����PSInit()����ֵ
* @param [in] strSaveFileName ����PSD�ļ�ȫ·��
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSExportPSDFile(const void* core, std::string& strSaveFileName, std::string& outMsg);

/**
 * @brief ��ȡ��ǰ�����ĵ���ͼ
 * @param [in] core ����PSInit()����ֵ
 * @param [in] width ָ����ȡͼƬ���
 * @param [in] height ָ����ȡͼƬ�߶�
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSGetDocumentImage(const void* core, int width, int height, std::string& outMsg);

 /**
 * @brief ��ȡ����PSD�ļ���ǰ����ͼ
 * @param [in] core ����PSInit()����ֵ
 * @param [in] width ָ����ȡͼƬ���
 * @param [in] height ָ����ȡͼƬ�߶�
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSGetForegroundLayerImage(const void* core, int width, int height, std::string& outMsg);

 /**
 * @brief ��ȡ����PSD�ļ��б�����ͼ
 * @param [in] core ����PSInit()����ֵ
 * @param [in] width ָ����ȡͼƬ���
 * @param [in] height ָ����ȡͼƬ�߶�
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSGetBackgroundLayerImage(const void* core, int width, int height, std::string& outMsg);

/**
 * @brief ����ѡ���ͼ�㲢��������
 * @param [in] core ����PSInit()����ֵ
 * @param [in] strNewLayerName ������ͼ�������
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSUnlockSelectLayer(const void* core, std::string strNewLayerName, std::string& outMsg);

/**
* @brief ɾ��ָ�����Ƶ�ͼ��
* @param [in] core ����PSInit()����ֵ
* @param [in] strLayerName ��ɾ��ͼ�������
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSDeleteSelectLayer(const void* core, std::string strLayerName, std::string& outMsg);

/**
 * @brief ��ָ��ͼ�㴴������/�Աȶ�ͼ��
 * @param [in] core ����PSInit()����ֵ
 * @param [in] strLayerName ��Ҫ��������/�Աȶȵ�ͼ������
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSCreateContrastLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
 * @brief �������ȺͶԱȶ�
 * @param [in] nBrigh ���ȣ���Χ��-150 ~ 150
 * @param [in] nContr �Աȶȣ���Χ��-50 ~ 100
 * @param [out] outMsg ����ִ����ɺ���
 * @return ״̬�� 0-�ɹ�
 */
PS_EXPORT_API int PSSetContrastLayerByName(const void* core, int nBrigh, int nContr, std::string& outMsg);

/**
* @brief ��ָ��ͼ�㴴��ɫ��ƽ��ͼ��
* @param [in] core ����PSInit()����ֵ
* @param [in] strLayerName ��Ҫ����ɫ��ƽ���ͼ������
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSCreateColorBalanceLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
* @brief ����ɫ��ƽ��
* @param [in] v1 �ݲ�ʹ�ã�Ĭ�ϣ�0
* @param [in] v2 ɫ��ֵ����Χ��-100 ~ 100
* @param [in] v3 ɫ��ֵ����Χ��-100 ~ 100
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSSetColorBalanceLayerByName(const void* core, int v1, int v2, int v3, std::string& outMsg);

/**
* @brief ��ָ��ͼ�㴴�����Ͷ�ͼ��
* @param [in] core ����PSInit()����ֵ
* @param [in] strLayerName ��Ҫ����ɫ��ƽ���ͼ������
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSCreateSaturabilityLayerByName(const void* core, std::string strLayerName, std::string& outMsg);

/**
* @brief ���ñ��Ͷ�
* @param [in] core ����PSInit()����ֵ
* @param [in] nSatur ɫ��ֵ����Χ��-100 ~ 100
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSSetSaturabilityLayerByName(const void* core, int nSatur, std::string& outMsg);

/**
* @brief ���û���ɫ
* @param [in] core ����PSInit()����ֵ
* @param [in] strLayerName ��Ҫ���û���ɫ��ͼ������
* @param [in] fRed ��ɫ����Χ��0 ~ 255
* @param [in] fGreen ��ɫ����Χ��0 ~ 255
* @param [in] fBlue ��ɫ����Χ��0 ~ 255
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSSetAmbientColor(const void* core, std::string strLayerName, float fRed, float fGreen, float fBlue, std::string& outMsg);

/**
* @brief ��ָ��ͼƬ���п�ͼ����
* @param [in] core ����PSInit()����ֵ
* @param [in] strLayerName ��Ҫ��ͼ��ͼ������
* @param [in] eCutoutType ����ͼͼƬ�ı�����ɫ����
* @param [out] outMsg ����ִ����ɺ���
* @return ״̬�� 0-�ɹ�
*/
PS_EXPORT_API int PSCutoutByName(const void* core, std::string strLayerName, ECUTOUTTYPE eCutoutType, std::string& outMsg);

#endif