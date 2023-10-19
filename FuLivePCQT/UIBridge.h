﻿#ifndef UIBRIDGE_H
#define UIBRIDGE_H
#include <QObject>
#include "Config.h"
#include <QStringList>
#include <QVariant>
#include <QDebug>
#include <QCameraInfo>
#include "ImageProvider.h"
#include "Nama.h"
#include <QMediaPlayer>
#include <QVector3D>
#include <Camera.h>
using namespace std;

//鉴权证书页1
enum CERTIFACITE_TYPE
{
    DEFINE_Face_Beautify,
    DEFINE_2D_Sticker,
    DEFINE_3D_Sticker,
    DEFINE_Skeletal_animation,
    DEFINE_Avatar,
    DEFINE_AR_Mask,
    DEFINE_AR_Mask_HD,
    DEFINE_Background_Segmentation,
    DEFINE_Gesture_Recognition,
    DEFINE_Video_Filter,
    DEFINE_Expression_Recognition,
    DEFINE_P2A_Prime,
    DEFINE_P2A_Creative,
    DEFINE_Portrait_Relighting,
    DEFINE_Facewarp,
    DEFINE_Music_Filter,
    DEFINE_Emotion_Recognition,
    DEFINE_Make_Up,
    DEFINE_Hair_Color,
    DEFINE_Cartoon_Filter,
    DEFINE_Face_Fusion,
    DEFINE_Voice_Lite,
    DEFINE_Voice,
    DEFINE_P2A_Gen_Head,
    DEFINE_P2A_Gen_Hair,
    DEFINE_P2A_Knead_Face,
    DEFINE_P2A_Get_Faceinfo,
    DEFINE_SIZE
};
static const int define_arr[DEFINE_SIZE] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x100,0x200,
0x400,0x800,0x1000,0x2000,0x4000,0x10000,0x20000,0x40000,0x80000,
0x100000,0x200000,0x800000,0x2000000,0x4000000,0x8000000,0x10000000,0x20000000,0x40000000 };

//鉴权证书页2
enum CERTIFACITE_TYPE_EXT
{
    DEFINE_P2A_IMAGE,
    DEFINE_P2A_VIDEO,
    DEFINE_P2A_GIF,
    DEFINE_LIGHT_MAKEUP,
    DEFINE_NAMA_KNEAD_FACE,
    DEFINE_BODY_SHAPE,
    DEFINE_RENDER,
    DEFINE_BODY_TRACK,
    DEFINE_BODY_TRACK_GESTURE,
    DEFINE_GREEN_SCREEN_EDIT,
    DEFINE_PUSH_STREAM,
    DEFINE_GET_FACEINFO,
    DEFINE_FACE_LANDMARK,
    DEFINE_EXPRESSION,
    DEFINE_2D_BODY_LANDMARKS,
    DEFINE_HEAD_SEGMENTATION,
    DEFINE_MOTION_RECOGNITION,
    DEFINE_BOUTIQUE_STICKER,
    DEFINE_ITEM_OFFLINE_SIGN,
    DEFINE_PK_GAME,
    DEFINE_STYLE_RECOMMENDATION,
    DEFINE_SIZE_EXT
};
static const int define_arr_ext[DEFINE_SIZE_EXT] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,
0x10000,0x20000,0x40000,0x80000, 0x100000};

static const int g_checkIndex[Count] = {
    1,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1
};

static const int g_checkID[Count] =
{
    define_arr_ext[DEFINE_BOUTIQUE_STICKER],define_arr[DEFINE_2D_Sticker],define_arr_ext[DEFINE_STYLE_RECOMMENDATION],
    define_arr[DEFINE_Make_Up], define_arr_ext[DEFINE_LIGHT_MAKEUP],define_arr[DEFINE_Background_Segmentation],
    define_arr[DEFINE_Gesture_Recognition],define_arr[DEFINE_Expression_Recognition],define_arr[DEFINE_Avatar],
    define_arr_ext[DEFINE_BODY_TRACK],define_arr[DEFINE_Hair_Color], define_arr[DEFINE_AR_Mask], define_arr[DEFINE_Music_Filter],
    define_arr[DEFINE_Facewarp],define_arr_ext[DEFINE_HEAD_SEGMENTATION], define_arr_ext[DEFINE_GREEN_SCREEN_EDIT]
};

enum SideCategory {
    BeautifyFaceSkin,
    BeautifyFaceShape,
    BeautifyFilter,
    BeautifyBody,
    SideCount
};

static const int g_checkSideIndex[SideCount] = {
    0,0,0,1
};

static const int g_checkSideID[Count] =
{
    define_arr[DEFINE_Face_Beautify], define_arr[DEFINE_Face_Beautify],define_arr[DEFINE_Face_Beautify],
    define_arr_ext[DEFINE_BODY_SHAPE]
};

enum ItemParam{
    BeautySkin,             //美肤
    BeautyFace,             //美型
    BeautyBody,             //美体
    ItemGreenScreen         //绿幕
};

extern const string g_assetDir;
const QString gBundlePath[] = {
    QString::fromStdString(g_assetDir) + "items/" + "ItemJingpin", //精品贴纸联网下载,路径不可用
    QString::fromStdString(g_assetDir) + "items/" + "ItemSticker",
    QString::fromStdString(g_assetDir) + "items/" + "StyleRecommendation",
    QString::fromStdString(g_assetDir) + "items/" + "Makeup",
    QString::fromStdString(g_assetDir) + "items/" + "LightMakeup",
    QString::fromStdString(g_assetDir) + "items/" + "BackgroundSegmentation",
    QString::fromStdString(g_assetDir) + "items/" + "GestureRecognition",
    QString::fromStdString(g_assetDir) + "items/" + "ExpressionRecognition",
    QString::fromStdString(g_assetDir) + "items/" + "Animoji",
    QString::fromStdString(g_assetDir) + "Avatars/",
    QString::fromStdString(g_assetDir) + "items/" + "BeautyHair",
    QString::fromStdString(g_assetDir) + "items/" + "ARMask",
    QString::fromStdString(g_assetDir) + "items/" + "MusicFilter",
    QString::fromStdString(g_assetDir) + "items/" + "MagicMirror",
    QString::fromStdString(g_assetDir) + "items/" + "BigHead",
    QString::fromStdString(g_assetDir) + "items/" + "GreenScreen",
    QString::fromStdString(g_assetDir) + "items/" + "SafeArea"
};

struct StyleRecommendationParam{
    QStringList mNameList;              //风格推荐名称列表,按名称排序,ailing第一个
    QList<QVariant> mBeautySkinDefault; //美肤默认参数列表
    QList<QVariant> mBeautyFaceDefault; //美型默认参数列表
    QList<QVariant> mBeautySkin;        //美肤用户参数列表
    QList<QVariant> mBeautyFace;        //美型用户参数列表
    QList<int> mFilterLevel;            //滤镜强度
    QList<int> mMakeUpIntensity;        //美妆强度
};

class UIBridge:public QObject{
    Q_OBJECT
    Q_PROPERTY(QStringList cameraSetList READ cameraSetList NOTIFY cameraSetListChanged)
    Q_PROPERTY(QList<QVariant> categoryBundles READ categoryBundles)
    Q_PROPERTY(QStringList stickerTip READ stickerTip)
    Q_PROPERTY(QList<QVariant> greenScreenIcon READ greenScreenIcon)
    Q_PROPERTY(QList<QVariant> greenScreen READ greenScreen)
    Q_PROPERTY(QList<QVariant> beautySkin READ beautySkin)
    Q_PROPERTY(QList<QVariant> beautyFace READ beautyFace)
    Q_PROPERTY(QList<QVariant> beautyBody READ beautyBody)
    Q_PROPERTY(QList<QVariant> filter READ filter)
    Q_PROPERTY(QVariant gsKeyColor READ gsKeyColor)
    Q_PROPERTY(QVariant checkModule READ checkModule)
    Q_PROPERTY(bool arFunction READ arFunction WRITE setARFunction NOTIFY arFunctionChanged)
    Q_PROPERTY(QStringList cameraList READ cameraList NOTIFY cameraListChanged)
    Q_PROPERTY(int selectCategory READ selectCategory WRITE setSelectCategory)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(QString frameSize READ frameSize NOTIFY frameSizeChanged)
    Q_PROPERTY(int renderTime READ renderTime NOTIFY renderTimeChanged)
    Q_PROPERTY(QString tip READ tip NOTIFY tipChanged)
    Q_PROPERTY(QString tipExtra READ tipExtra NOTIFY tipExtraChanged)
    Q_PROPERTY(int bodyTrackType READ bodyTrackType WRITE setBodyTrackType NOTIFY bodyTrackTypeChanged)
    Q_PROPERTY(int cameraWidth WRITE setCameraWidth NOTIFY cameraWidthChanged)
    Q_PROPERTY(int cameraHeight WRITE setCameraHeight NOTIFY cameraHeightChanged)
    //    Q_PROPERTY(QString gsCameraName WRITE setGSCameraName NOTIFY gsCameraNameChanged)
    Q_PROPERTY(bool virturalCamera READ virturalCamera WRITE setVirturalCamera NOTIFY virturalCameraChanged)
    Q_PROPERTY(bool gsSelectCamera READ gsSelectCamera NOTIFY gsSelectCameraChanged)
    Q_PROPERTY(int selectCameraSet READ selectCameraSet NOTIFY selectCameraSetChanged)
    Q_PROPERTY(bool gsCameraImage WRITE setGSCameraImage)
    Q_PROPERTY(bool gsCameraPlay WRITE setGSCameraPlay)
public:
    //qml访问函数
    QStringList cameraSetList();
    QList<QVariant> categoryBundles() { return m_categoryBundles; }
    QStringList stickerTip();
    QList<QVariant> greenScreenIcon() { return m_greenScreenIcon; }
    QList<QVariant> greenScreen(){ return m_greenScreen; }
    QList<QVariant> beautySkin(){ return m_beautySkin; }
    QList<QVariant> beautyFace(){ return m_beautyFace; }
    QList<QVariant> beautyBody(){ return m_beautyBody; }
    QList<QVariant> filter(){ return m_filter; }
    QVariant gsKeyColor(){ return m_gsKeyColor; }
    QVariant checkModule(){ return m_checkMode; }
    QStringList cameraList();
    bool arFunction() { return m_arFunction; }
    int selectCategory(){ return  m_selectCategory;}
    void setARFunction(bool ar);
    void setSelectCategory(int index);
    int fps(){return m_FPS;}
    QString frameSize(){return m_frameSize;}
    int renderTime(){return m_renderTime;}
    QString tip(){return m_tip;}
    QString tipExtra(){return m_tipExtra;}
    int bodyTrackType(){return int(m_bodyTrackType);}
    void setBodyTrackType(int type);
    void setCameraWidth(int width){ m_cameraWidth = width;}
    void setCameraHeight(int height){ m_cameraHeight = height;}
    bool virturalCamera();
    void setVirturalCamera(bool vc);
    bool gsSelectCamera(){return m_gsSelectCamera;}
    int selectCameraSet(){return m_selectCameraSet;}
    void setGSCameraImage(bool flag);
    void setGSCameraPlay(bool flag);
public:
    UIBridge();
    ~UIBridge();
    //读取道具对应提示
    void readBundleTip();
    //读取道具模型
    void readCategoryBundle();
    //更新道具参数,place为QList<QVariant>的下标,index为替换字符下标,value替换字符值
    void updateCategory(QList<QVariant> &lv, int place, int index, QString value);
    //从配置文件中导入自定义美妆列表
    void readCustomMakeup();
    //从配置文件中导入风格推荐
    void readStyleRecommendation();
    //创建nama默认图,显示摄像头未连接
    void creatdefaultFrame();
    //初始化avator配置参数
    void initBodyTrackConfig();
    //解绑avatar
    void unLoadAvatar();
    //更新绿幕安全区域
    void updateGSSafeArea(QImage &image);
    //读取json配置
    void readUserConfig();
    //使用精品道具
    void useBoutique(int index);
    //重新加载美颜配置
    void reloadItemParam();
    //检测美体参数是否全为0
    void checkBodyParam();
public:
    //上一次打开摄像头的名称
    QString          m_cameraName;
    int              m_cameraSet = 0;
    //底部ar功能/绿幕抠像,true为ar功能
    bool             m_arFunction = true;
    //ar绿幕切换需要render新的一帧
    bool             m_renderNewFrame = true;
    //图像获得新的一帧
    bool             m_newImage = true;
    //精品贴纸道具
    QList<QVariant>  m_stickerHolder;
    //精品道具标题
    QStringList      m_stickerTip;
    //精品贴纸标题选中第几个
    int              m_stickerIndex = 0;
    //底部道具
    QList<QVariant>  m_categoryBundles;
    //绿幕底部按钮选项
    QList<QVariant>  m_greenScreenIcon;
    //绿幕参数值
    QList<QVariant>  m_greenScreen;
    //绿幕参数默认值
    QVariant         m_defaultGreenScreen;
    //右侧美肤
    QList<QVariant>  m_beautySkin;
    //美肤默认值
    QVariant         m_defaultBeautySkin;
    //右侧美型
    QList<QVariant>  m_beautyFace;
    //美型默认值
    QVariant         m_defaultBeautyFace;
    //右侧美体
    QList<QVariant>  m_beautyBody;
    //美体默认值
    QVariant         m_defaultBeautyBody;
    //用户自定义美颜设置
    QStringList      m_userCustomMakeup;
    //右侧滤镜
    QList<QVariant>  m_filter;
    //鉴权列表
    QVariant m_checkMode;
    //风格推荐参数
    StyleRecommendationParam m_styleRecommendationParam;
    //风格推荐选中第几个
    int m_styleRecommendationIndex = -1;
    //风格推荐名称排序列表,按展示排序,xuejie第一个
    QStringList m_styleRecommendationName;
    //绿幕最近使用颜色
    QVariant m_gsKeyColor;
    //临时美妆列表,最终存到m_customMakeup每个的第一条
    QList<QVariant>  m_tempCustomMakeup;
    //自定义美妆
    QList<QList<QVariant>>  m_customMakeup;
    //自定义美妆
    QList<QList<QVariant>>  m_defaultCustomMakeup;
    //选中第几个滤镜
    int              m_filterIndex = 0;
    //底部选中第几个道具项
    int              m_selectCategory = -1;
    //底部上次选中第几个道具项
    int              m_selectCategoryLast = -1;
    //弹出道具选中第几个
    int              m_selectCategoryIndex = -1;
    //显示帧数
    int              m_FPS = 0;
    //显示计算时间
    int              m_renderTime = 0;
    //道具对应提示文字
    QMap<QString,QString> m_tipMap;
    //qml显示提示文字
    QString          m_tip = "";
    QString          m_tipExtra = "";
    //显示AR功能跟美体模块无法共用 flag
    bool             m_flagARBody = false;
    //qml中显示avator或者绿幕图像
    ImageProvider    *m_ImageProvider;
    //需要显示绿幕摄像头图
    bool             m_bGSCameraImage = false;
    //qml中改变显示图像长宽
    int              m_cameraWidth = 864;
    int              m_cameraHeight = 486;
    //绿幕选中相机名称
    //QString          m_gsCameraName;
    //绿幕背景视频播放
    QMediaPlayer m_gsMediaPlayer;
    QtCameraCapture *m_gsvideoSurface;
    //绿幕视频播放
    QMediaPlayer m_gsVideoMediaPlayer;
    //背景分割视频播放
    QMediaPlayer m_bgsVideoMediaPlayer;
    //网络摄像头播放器
    QMediaPlayer m_webcamMediaPlayer;
    //自定义背景分割需要存图片
    bool m_bNeedSavebg_seg = false;
    //自定义背景分割所用图片或视频路径
    QString m_begUserFilePath = "";
    //自定义绿幕安全区域所用图片路径
    QString m_gsSafeUserFilePath = "";
    //绿幕最近使用颜色
    QList<QVector3D> m_gsKeyColorList;
    //绿幕图像初始位置
    QPointF m_gsStart = QPointF(0.5,0.5);
    QPointF m_gsSize = QPointF(0.5,0.5);
    //绿幕取色flag,取色时
    bool    m_bSelectColor = false;
    //下载精品贴纸线程
    std::thread m_GDownloadBundleThread;
    //是否下载精品贴纸
    bool  m_GIsDownloading = false;
    //选中绿幕摄像头还是选择视频/图片
    bool  m_gsSelectCamera = true;
    //选中选择视频/图片路径
    QString  m_gsSelectVideoPath = "";
    //绿幕选中第几个背景
    int   m_gsSelectBgIndex = 0;
    //绿幕选中第几个安全区域
    int   m_gsSelectSafeArea = -1;
    //摄像头参数选中第几个
    int   m_selectCameraSet = -1;
    //绿幕选择视频图片更改绿幕图像范围参数
    bool  m_bgsSelectVideo = false;
    //更新fps定时器
    QTimer *m_timer;
    //记录fps一秒render更新了几帧
    int m_FrameIDFPS = 0;
    //统计1秒render总用时
    int m_secondRenderTime = 0;
    //显示性能参数中图像长宽
    QString m_frameSize;
    //已加载avatar道具
    bool m_bLoadAvatar = false;
    //已加载小熊道具
    bool m_bLoadBear = false;
    //avator人物跟踪类型
    BodyTrackType m_bodyTrackType = BodyTrackType::None;
    //avator参数
    BodyTrackConfig m_bodyTrackConfig;
    //绿幕安全区域图
    cv::Mat m_matSafeArea;
    //自定义口红水润特有,makeup_lip_color_v2替换makeup_lip_color
    bool m_bmakeup_moisturized = false;
    //绿幕抠像选择颜色,默认绿色
    cv::Vec4b m_gsColor = {0,255,0,255};
    //是否为老美妆道具,需要绑到Makeup
    bool m_bmakeupFlag = true;
    //轻美妆道具名称
    QString m_lightMakeUpName;
    //摄像头类型,true为网络摄像头,false为usbs摄像头
    bool m_CameraType = false;
    //绿幕选择摄像头小窗口播放
    bool m_gsCameraPlay = false;
    //风格推荐保存flag,更新参数时不保存
    bool m_bSaveStyleRecommendation = false;
    //更改背景分割模式需要与渲染同线程
    int m_bSetBackgroundSegType = -1;
signals:
    //qml界面相关
    void cameraSetListChanged();
    void arFunctionChanged();
    void cameraListChanged();
    void frameSizeChanged();
    void fpsChanged();
    void renderTimeChanged();
    void tipChanged(int time = 2);
    void tipExtraChanged(int time = 2);
    void arBodyFlagChanged(bool arBody);
    void showAvatorChanged();
    void bodyTrackTypeChanged();
    //通知qml刷新avator,绿幕图片
    void callQmlRefeshImg();
    void cameraWidthChanged();
    void cameraHeightChanged();
    void selectColorChanged(QString);
    void switchARChanged(bool);
    void virturalCameraChanged();
    void finishDownload(int);
    void gsSelectCameraChanged();
    void selectCameraSetChanged();
    //通知qml刷新自定义背景分割图片
    void updateBsgPic();
    //通知qml刷新自定义绿幕安全区域图片
    void updateGSSafePic();
    //通知主线程更新配置
    void updateConfig();
    //qml更新选中相机
    void updateCameraIndex(int index);
    void updateCameraSet(int index);
    void updateBodyTrackType(int type);
    void updateSelectCategory(int x, int y, int z);
    void updateGSSelectIndex(int x, int y);
    //更新绿幕实际图位置区域
    void updateGSPreviewRect(float startx, float starty, float endx, float endy);
    //更新美肤参数
    void updateBeautySkinParam();
    //更新美型参数
    void updateBeautyFaceParam();
    //更新绿幕参数
    void updateGreenScreenParam();
    //更新绿幕重播按钮状态
    void updateReplayButton(bool flag);
    //更新ui鉴权
    void updateCheckModule();
public slots:
    //界面开启获取相机列表
    void getCameraList();
    //从摄像头读取到一帧,保存一个实时frame
    void getPresentFrame(const cv::Mat &frame);
    //从绿幕视频从读取到一帧
    void getGSPresentFrame(const cv::Mat &frame);
    //更新绿幕背景
    void updateGreenScreenSegment(const cv::Mat & dataRGBA);
    //更新背景分割背景
    void updateBackgroundSegment(const cv::Mat & dataRGBA);
    //qml调用使用道具
    void useProps(int index);
    //qml调用不使用当前道具
    void nonuseProps();
    //qml中调用更改参数 美肤,美型,美体,绿幕通用
    void updateItemParam(int item, int index, QString value);
    //qml中调用重置参数
    void resetItemParam(int item);
    //调用nama函数
    void namaFuItemSetParamd(int handle, QList<QVariant> &valueList, int index, bool flag);
    //qml中滤镜参数值
    void setFilter(QString value);
    //更改滤镜选中
    void resetFilterIndex(int index);
    //访问自定义美妆二维数组
    QList<QVariant> getCustomMakeup(int index){ return m_customMakeup.at(index); }
    //QString颜色转vector #FFFFFFFF转(255,255,255,255)
    static std::vector<double> GetColorDouble(QString color);
    //qml修改自定义美妆滑块值
    void setCustomMakeupValue(int index, QString value);
    //qml修改自定义美妆选中项目
    void setCustomMakeupIndex(int index, QString value);
    //qml修改自定义美妆选中颜色
    void setCustomMakeupColor(int index, QString value);
    //一键卸妆
    void resetCustomMakeup();
    //摄像头选择更改
    void changeCamera(int index);
    //摄像头参数选择更改
    void changeCameraSet(int index);
    //绿幕选择摄像头确定
    void gsCameraConfirm();
    //取色点击
    void selectColor(int mouseX, int mouseY);
    //绿幕保存最近使用颜色
    void saveGSKeyColor(QVector3D color3d);
    //qml中绿幕颜色圈颜色变化
    void gsColorChange(QString color);
    //绿幕获取移动后位置
    QPointF gsLocation(double moveX, double moveY);
    //绿幕改变位置
    void gsTranslocation(double moveX, double moveY);
    //绿幕窗口放大缩小,true放大
    void gsZoom(bool zoom);
    //绿幕松开鼠标更新初始位置
    void gsUpdatelocation(double moveX, double moveY);
    //绿幕播放背景视频
    void gsPlayBGVideo(QString videopath);
    //设置绿幕取色状态
    void gsSelectColor();
    //设置绿幕取消取色状态
    void gsUnSelectColor();
    //绿幕打开视频
    void gsSelectVideo(QString path);
    //背景分割选择视频
    void bgsSelectVideo(QString path);
    //绿幕安全区域选择
    void gsSafeAreaSelect(QString path);
    //打开帮助文件
    void openHelpText();
    //得到中级道具或高级道具数量
    int getStickerLength();
    //获取贴纸设置值
    QVariant getStickerValue(int index, int n);
    //设置精品贴纸选中第几个
    void setStickerIndex(int index){ m_stickerIndex = index;}
    //下载贴纸
    void downloadSticker(int index);
    //精品道具,马赛克道具点击触发
    void itemJingpinClick();
    //每秒显示帧数和计算时间
    void getFPS();
    //打开web摄像头
    void openWebCamera(QString path);
    //暂停或开始web摄像头
    void stopStartWebCamera(bool flag);
    //改变摄像头类型,true为网络摄像头,false为usbs摄像头
    void changeCameraType(bool type);
    //保存用户操作配置,默认只保存自定义背景图、绿幕安全图、绿幕最近使用颜色,true全保存
    void saveUserConfig(bool flag = false);
    //更新用户操作配置,界面参数到nama中
    void updateUserConfig();
    //选择绿幕输入重新视频播放
    void startMediaPlayer(){
        if(m_gsSelectCamera == false){
            m_gsVideoMediaPlayer.play();
        }
    }
    //选择绿幕输入暂停视频播放
    void stopMediaPlayer(){
        m_gsVideoMediaPlayer.stop();
    }
    //轻美妆设置参数
    void setLightMakeUpParam(QString name);
    //轻美妆加载设置图片
    void setLightMakeUpTex(string value,string imagepath);
    //轻美妆设置口红颜色
    void setLightMakeUpLipColor(string colorpath);
    //更新滤镜
    void updateFilter();
    //更新风格推荐参数
    void updateStyleRecommendation(int index = -1);
    //保存风格推荐
    void saveStyleRecommendation();
    //绿幕切换卡一帧
    void setRenderNewFrame(){m_renderNewFrame = false; m_newImage = false;}
    //设置背景分割会议版 0|通用版 1
    void setBackgroundSegType(int type);
    //绿幕视频播放状态改变
    void changedStatus(QMediaPlayer::MediaStatus);
    //绿幕视频重新播放
    void gsVideoMediaReplay(){ m_gsVideoMediaPlayer.play(); }
    //鉴权验证
    bool checkModuleCode(int category);
    bool checkModuleCodeSide(int side);
    //显示检测不到人脸提示
    void detectionTip();
    //显示检测不到人体提示
    void detectionBodyTip();
    //右侧美肤,美型切换
    void updateTapContent(int index);
};

#endif // UIBRIDGE_H
