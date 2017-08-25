﻿#include "imagesegmentation.h"


#include "imagegray.h"

using namespace std;
// 大津阈值获取
QImage * ImageSegmentation::ostu(QImage &image)
{
    // 方差最大时说明图片的前后背景色的划分为最合适
    QVector<int> grayHist = ImageGray::countGrayHistogram(&image);

    int N = image.width() * image.height(); //图片总的像素点
    int N0 = 0; // 背景色所占的像素的总数
    int N1 = 0; // 前景色所占的像素的总数
    int max = 0; // 最大方差所取时的灰度值
    double gMax = 0; // 最大方差

    for(int i = 0; i < 256; i++){

        N0 += grayHist[i];
        N1 = N - N0;

        if(N1 == 0){
            break;
        }

        double w0 = N0 * 1.0 / N;
        double w1 = 1 - w0;

        int sum = 0;
        for(int m = 0; m <= i; m++){
            sum += m * grayHist[i];
        }
        double u0 = sum * 1.0 / N0;

        sum = 0;
        for( int n = i + 1; n < 256; n++){
            sum += n * grayHist[i];
        }
        double u1 = sum * 1.0 / N1;

        double g = w0*w1*(u0-u1)*(u0-u1);

        if(g > gMax){
            gMax = g;
            max = i;
        }

    }

    QImage * newImage = ImageGray::binaryzation(image, max);

    return newImage;

}


//所有算子对阈值是否应该有个判断
QImage *ImageSegmentation::RobertOperator(QImage *img)
{
    QImage * newImg = new QImage(*img);
    int newHeight = newImg->height(),
            newWidth = newImg->width();
//读取图像中的像素灰度存入两层vector构成的矩阵
    vector<vector<float>> tmpR;
    vector<vector<float>> tmpG;
    vector<vector<float>> tmpB;
    ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
    for(int i = 0;i < newHeight-1;++i)
    {
        for(int j =0;j< newWidth-1;++j)
        {
            tmpR[i][j] = abs(tmpR[i][j]-tmpR[i+1][j+1])+abs(tmpR[i+1][j]-tmpR[i][j+1]);
            tmpR[i][j] = tmpR[i][j]>255?255:tmpR[i][j];
            tmpG[i][j] = abs(tmpG[i][j]-tmpG[i+1][j+1])+abs(tmpG[i+1][j]-tmpG[i][j+1]);
            tmpG[i][j] = tmpG[i][j]>255?255:tmpG[i][j];
            tmpB[i][j] = abs(tmpB[i][j]-tmpB[i+1][j+1])+abs(tmpB[i+1][j]-tmpB[i][j+1]);
            tmpB[i][j] = tmpB[i][j]>255?255:tmpB[i][j];
        }
    }
    //转回图像
    ImagTranslate::vector2ColorImage(tmpR,tmpG,tmpB,*newImg);
    return newImg;

}



QImage *ImageSegmentation::SobelOperator(QImage *img)
{
    QImage * newImg = new QImage(*img);
    int newHeight = newImg->height(),
            newWidth = newImg->width();
    vector<vector<float>> tmpR;
    vector<vector<float>> tmpG;
    vector<vector<float>> tmpB;
    ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
    vector<vector<float>> resultR = tmpR;
    vector<vector<float>> resultG = tmpG;
    vector<vector<float>> resultB = tmpB;
    float resultRX,resultRY;
    float resultGX,resultGY;
    float resultBX,resultBY;
    for(int i = 1;i < newHeight-1;++i)
    {
        for(int j =1;j< newWidth-1;++j)
        {
            resultRX      = tmpR[i-1][j+1] + 2*tmpR[i][j+1] + tmpR[i+1][j+1]-(tmpR[i-1][j-1] + 2*tmpR[i][j-1] + tmpR[i+1][j-1]);
            resultRY      = tmpR[i+1][j+1] + 2*tmpR[i+1][j] + tmpR[i+1][j-1]-(tmpR[i-1][j+1] + 2*tmpR[i-1][j] + tmpR[i-1][j-1]);
            resultR[i][j] = sqrt( resultRX* resultRX+ resultRY* resultRY);
            resultR[i][j] = resultR[i][j]>255?255:resultR[i][j];
            resultGX      = tmpG[i-1][j+1] + 2*tmpG[i][j+1] + tmpG[i+1][j+1]-(tmpG[i-1][j-1] + 2*tmpG[i][j-1] + tmpG[i+1][j-1]);
            resultGY      = tmpG[i+1][j+1] + 2*tmpG[i+1][j] + tmpG[i+1][j-1]-(tmpG[i-1][j+1] + 2*tmpG[i-1][j] + tmpG[i-1][j-1]);
            resultG[i][j] = sqrt( resultGX* resultGX+ resultGY* resultGY);
            resultG[i][j] = resultG[i][j]>255?255:resultG[i][j];
            resultBX      = tmpB[i-1][j+1] + 2*tmpB[i][j+1] + tmpB[i+1][j+1]-(tmpB[i-1][j-1] + 2*tmpB[i][j-1] + tmpB[i+1][j-1]);
            resultBY      = tmpB[i+1][j+1] + 2*tmpB[i+1][j] + tmpB[i+1][j-1]-(tmpB[i-1][j+1] + 2*tmpB[i-1][j] + tmpB[i-1][j-1]);
            resultB[i][j] = sqrt( resultBX* resultBX+ resultBY* resultBY);
            resultB[i][j] = resultB[i][j]>255?255:resultB[i][j];
        }
    }
    //转回图像
    ImagTranslate::vector2ColorImage(resultR,resultG,resultB,*newImg);
    delete img;
    img = NULL;
    return newImg;
}

QImage *ImageSegmentation::PrewittOperator(QImage *img)
{

    QImage * newImg = new QImage(*img);
    int newHeight = newImg->height(),
            newWidth = newImg->width();
    vector<vector<float>> tmpR;
    vector<vector<float>> tmpG;
    vector<vector<float>> tmpB;
    ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
    vector<vector<float>> resultR = tmpR;
    vector<vector<float>> resultG = tmpG;
    vector<vector<float>> resultB = tmpB;
    float resultRX,resultRY;
    float resultGX,resultGY;
    float resultBX,resultBY;
    for(int i = 1;i < newHeight-1;++i)
    {
        for(int j =1;j< newWidth-1;++j)
        {
            resultRX      = tmpR[i-1][j+1] + tmpR[i][j+1] + tmpR[i+1][j+1]-(tmpR[i-1][j-1] + tmpR[i][j-1] + tmpR[i+1][j-1]);
            resultRY      = tmpR[i+1][j+1] + tmpR[i+1][j] + tmpR[i+1][j-1]-(tmpR[i-1][j+1] + tmpR[i-1][j] + tmpR[i-1][j-1]);
            resultR[i][j] = sqrt( resultRX* resultRX+ resultRY* resultRY);
            resultR[i][j] = resultR[i][j]>255?255:resultR[i][j];
            resultGX      = tmpG[i-1][j+1] + tmpG[i][j+1] + tmpG[i+1][j+1]-(tmpG[i-1][j-1] + tmpG[i][j-1] + tmpG[i+1][j-1]);
            resultGY      = tmpG[i+1][j+1] + tmpG[i+1][j] + tmpG[i+1][j-1]-(tmpG[i-1][j+1] + tmpG[i-1][j] + tmpG[i-1][j-1]);
            resultG[i][j] = sqrt( resultGX* resultGX+ resultGY* resultGY);
            resultG[i][j] = resultG[i][j]>255?255:resultG[i][j];
            resultBX      = tmpB[i-1][j+1] + tmpB[i][j+1] + tmpB[i+1][j+1]-(tmpB[i-1][j-1] + tmpB[i][j-1] + tmpB[i+1][j-1]);
            resultBY      = tmpB[i+1][j+1] + tmpB[i+1][j] + tmpB[i+1][j-1]-(tmpB[i-1][j+1] + tmpB[i-1][j] + tmpB[i-1][j-1]);
            resultB[i][j] = sqrt( resultBX* resultBX+ resultBY* resultBY);
            resultB[i][j] = resultB[i][j]>255?255:resultB[i][j];
        }
    }
    //转回图像
     ImagTranslate::vector2ColorImage(resultR,resultG,resultB,*newImg);
     delete img;
     img = NULL;
    return newImg;
}



//    Laplace算子对孤立象素的响应要比对边缘或线的响应要更强烈，因此只适用于无噪声图象。存在噪声情况下，使用Laplacian算子检测边缘之前需要先进行低通滤波
QImage *ImageSegmentation::LaplacianOperator(QImage *img)
{
    QImage * newImg = new QImage(*img);
    int newHeight = newImg->height(),
            newWidth = newImg->width();
    vector<vector<float>> tmpR;
    vector<vector<float>> tmpG;
    vector<vector<float>> tmpB;
    ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
    vector<vector<float>> resultR = tmpR;
    vector<vector<float>> resultG = tmpG;
    vector<vector<float>> resultB = tmpB;
    for(int i = 1;i < newHeight-1;++i)
    {
        for(int j =1;j< newWidth-1;++j)
        {

            resultR[i][j] = tmpR[i+1][j] + tmpR[i-1][j] + tmpR[i][j+1] + tmpR[i][j-1] - 4*tmpR[i][j];
            resultG[i][j] = tmpG[i+1][j] + tmpG[i-1][j] + tmpG[i][j+1] + tmpG[i][j-1] - 4*tmpG[i][j];
            resultB[i][j] = tmpB[i+1][j] + tmpB[i-1][j] + tmpB[i][j+1] + tmpB[i][j-1] - 4*tmpB[i][j];
            resultR[i][j] = resultR[i][j]>0?(resultR[i][j]>255?255:resultR[i][j]):0;
            resultG[i][j] = resultG[i][j]>0?(resultG[i][j]>255?255:resultG[i][j]):0;
            resultB[i][j] = resultB[i][j]>0?(resultB[i][j]>255?255:resultB[i][j]):0;
        }
    }
    //转回图像
     ImagTranslate::vector2ColorImage(resultR,resultG,resultB,*newImg);
     delete img;
     img = NULL;
     return newImg;
}

QImage *ImageSegmentation::GaussLaplacianOperator(QImage *img)
{
   int Template1[5][5] =  { 0, 0, -1, 0, 0,
                            0, -1, -2, -1, 0,
                            -1, -2, 16,-2, -1,
                            0, -1, -2, -1, 0,
                            0, 0, -1, 0, 0};
   QImage * newImg = new QImage(*img);
   int newHeight = newImg->height(),
           newWidth = newImg->width();
   vector<vector<float>> tmpR;
   vector<vector<float>> tmpG;
   vector<vector<float>> tmpB;
   ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
   vector<vector<float>> resultR = tmpR;
   vector<vector<float>> resultG = tmpG;
   vector<vector<float>> resultB = tmpB;
   for(int i = 2;i < newHeight-2;++i)
   {
       for(int j =2;j< newWidth-2;++j)
       {
           resultR[i][j] = 0;
           resultG[i][j] = 0;
           resultB[i][j] = 0;
           for(int k =-2;k< 3;++k)
           {
               for(int l =-2;l< 3;++l)
               {

                   resultR[i][j] +=Template1[k+2][l+2]*tmpR[i+k][j+l];
                   resultG[i][j] +=Template1[k+2][l+2]*tmpG[i+k][j+l];
                   resultB[i][j] +=Template1[k+2][l+2]*tmpB[i+k][j+l];
               }
           }
           resultR[i][j] = resultR[i][j]>0?(resultR[i][j]>255?255:resultR[i][j]):0;
           resultG[i][j] = resultG[i][j]>0?(resultG[i][j]>255?255:resultG[i][j]):0;
           resultB[i][j] = resultB[i][j]>0?(resultB[i][j]>255?255:resultB[i][j]):0;
       }
   }
   //转回图像
   ImagTranslate::vector2ColorImage(resultR,resultG,resultB,*newImg);
   delete img;
   img = NULL;
   return newImg;
}



//这个算子速度很慢，需要调整
QImage *ImageSegmentation::KrischOperator(QImage *img)
{
        int Template1[3][3] = {  5,    5,    5,
                                -3,    0,   -3,
                                -3,   -3,  -3 };
        int Template2[3][3] = { -3,    5,    5,
                                -3,    0,    5,
                                -3,    -3,   -3 };
        int Template3[3][3] = { -3,    -3,   5,
                                -3,     0,   5,
                                -3,    -3,   5 };
        int Template4[3][3] = { -3,    -3,   -3,
                                -3,     0,    5,
                                -3,     5,    5 };
        int Template5[3][3] = { -3,    -3,    -3,
                                -3,     0,    -3,
                                 5,     5,     5 };
        int Template6[3][3] = {  -3,   -3,    -3,
                               5,    0,    -3,
                               5,    5,    -3 };
        int Template7[3][3] = {   5,    -3,   -3,
                               5,     0,   -3,
                               5,    -3,   -3 };
        int Template8[3][3] = {   5,     5,   -3,
                               5,     0,   -3,
                              -3,    -3,   -3 };

        QImage * newImg = new QImage(*img);
        int newHeight = newImg->height(),
                newWidth = newImg->width();
        vector<vector<float>> tmpR;
        vector<vector<float>> tmpG;
        vector<vector<float>> tmpB;
        ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
        vector<vector<float>> resultR1 = tmpR;
        vector<vector<float>> resultG1 = tmpG;
        vector<vector<float>> resultB1 = tmpB;

        vector<vector<float>> resultR2 = tmpR;
        vector<vector<float>> resultG2 = tmpG;
        vector<vector<float>> resultB2 = tmpB;

        vector<vector<float>> resultR3 = tmpR;
        vector<vector<float>> resultG3 = tmpG;
        vector<vector<float>> resultB3 = tmpB;

        vector<vector<float>> resultR4 = tmpR;
        vector<vector<float>> resultG4 = tmpG;
        vector<vector<float>> resultB4 = tmpB;

        vector<vector<float>> resultR5 = tmpR;
        vector<vector<float>> resultG5 = tmpG;
        vector<vector<float>> resultB5 = tmpB;

        vector<vector<float>> resultR6 = tmpR;
        vector<vector<float>> resultG6 = tmpG;
        vector<vector<float>> resultB6 = tmpB;

        vector<vector<float>> resultR7 = tmpR;
        vector<vector<float>> resultG7 = tmpG;
        vector<vector<float>> resultB7 = tmpB;

        vector<vector<float>> resultR8 = tmpR;
        vector<vector<float>> resultG8 = tmpG;
        vector<vector<float>> resultB8 = tmpB;

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR1[i][j] = 0;
                resultG1[i][j] = 0;
                resultB1[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR1[i][j] +=Template1[k+1][l+1]*tmpR[i+k][j+l];
                        resultG1[i][j] +=Template1[k+1][l+1]*tmpG[i+k][j+l];
                        resultB1[i][j] +=Template1[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR1[i][j] = resultR1[i][j]>0?(resultR1[i][j]>255?255:resultR1[i][j]):0;
                resultG1[i][j] = resultG1[i][j]>0?(resultG1[i][j]>255?255:resultG1[i][j]):0;
                resultB1[i][j] = resultB1[i][j]>0?(resultB1[i][j]>255?255:resultB1[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR2[i][j] = 0;
                resultG2[i][j] = 0;
                resultB2[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR2[i][j] +=Template2[k+1][l+1]*tmpR[i+k][j+l];
                        resultG2[i][j] +=Template2[k+1][l+1]*tmpG[i+k][j+l];
                        resultB2[i][j] +=Template2[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR2[i][j] = resultR2[i][j]>0?(resultR2[i][j]>255?255:resultR2[i][j]):0;
                resultG2[i][j] = resultG2[i][j]>0?(resultG2[i][j]>255?255:resultG2[i][j]):0;
                resultB2[i][j] = resultB2[i][j]>0?(resultB2[i][j]>255?255:resultB2[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {

                resultR3[i][j] = 0;
                resultG3[i][j] = 0;
                resultB3[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR3[i][j] +=Template3[k+1][l+1]*tmpR[i+k][j+l];
                        resultG3[i][j] +=Template3[k+1][l+1]*tmpG[i+k][j+l];
                        resultB3[i][j] +=Template3[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR3[i][j] = resultR3[i][j]>0?(resultR3[i][j]>255?255:resultR3[i][j]):0;
                resultG3[i][j] = resultG3[i][j]>0?(resultG3[i][j]>255?255:resultG3[i][j]):0;
                resultB3[i][j] = resultB3[i][j]>0?(resultB3[i][j]>255?255:resultB3[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR4[i][j] = 0;
                resultG4[i][j] = 0;
                resultB4[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR4[i][j] +=Template4[k+1][l+1]*tmpR[i+k][j+l];
                        resultG4[i][j] +=Template4[k+1][l+1]*tmpG[i+k][j+l];
                        resultB4[i][j] +=Template4[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR4[i][j] = resultR4[i][j]>0?(resultR4[i][j]>255?255:resultR4[i][j]):0;
                resultG4[i][j] = resultG4[i][j]>0?(resultG4[i][j]>255?255:resultG4[i][j]):0;
                resultB4[i][j] = resultB4[i][j]>0?(resultB4[i][j]>255?255:resultB4[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR5[i][j] = 0;
                resultG5[i][j] = 0;
                resultB5[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR5[i][j] +=Template5[k+1][l+1]*tmpR[i+k][j+l];
                        resultG5[i][j] +=Template5[k+1][l+1]*tmpG[i+k][j+l];
                        resultB5[i][j] +=Template5[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR5[i][j] = resultR5[i][j]>0?(resultR5[i][j]>255?255:resultR5[i][j]):0;
                resultG5[i][j] = resultG5[i][j]>0?(resultG5[i][j]>255?255:resultG5[i][j]):0;
                resultB5[i][j] = resultB5[i][j]>0?(resultB5[i][j]>255?255:resultB5[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR6[i][j] = 0;
                resultG6[i][j] = 0;
                resultB6[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR6[i][j] +=Template6[k+1][l+1]*tmpR[i+k][j+l];
                        resultG6[i][j] +=Template6[k+1][l+1]*tmpG[i+k][j+l];
                        resultB6[i][j] +=Template6[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR6[i][j] = resultR6[i][j]>0?(resultR6[i][j]>255?255:resultR6[i][j]):0;
                resultG6[i][j] = resultG6[i][j]>0?(resultG6[i][j]>255?255:resultG6[i][j]):0;
                resultB6[i][j] = resultB6[i][j]>0?(resultB6[i][j]>255?255:resultB6[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR7[i][j] = 0;
                resultG7[i][j] = 0;
                resultB7[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR7[i][j] +=Template7[k+1][l+1]*tmpR[i+k][j+l];
                        resultG7[i][j] +=Template7[k+1][l+1]*tmpG[i+k][j+l];
                        resultB7[i][j] +=Template7[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR7[i][j] = resultR7[i][j]>0?(resultR7[i][j]>255?255:resultR7[i][j]):0;
                resultG7[i][j] = resultG7[i][j]>0?(resultG7[i][j]>255?255:resultG7[i][j]):0;
                resultB7[i][j] = resultB7[i][j]>0?(resultB7[i][j]>255?255:resultB7[i][j]):0;
            }
        }

        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                resultR8[i][j] = 0;
                resultG8[i][j] = 0;
                resultB8[i][j] = 0;
                for(int k =-1;k< 2;++k)
                {
                    for(int l =-1;l< 2;++l)
                    {

                        resultR8[i][j] +=Template8[k+1][l+1]*tmpR[i+k][j+l];
                        resultG8[i][j] +=Template8[k+1][l+1]*tmpG[i+k][j+l];
                        resultB8[i][j] +=Template8[k+1][l+1]*tmpB[i+k][j+l];
                    }
                }
                resultR8[i][j] = resultR8[i][j]>0?(resultR8[i][j]>255?255:resultR8[i][j]):0;
                resultG8[i][j] = resultG8[i][j]>0?(resultG8[i][j]>255?255:resultG8[i][j]):0;
                resultB8[i][j] = resultB8[i][j]>0?(resultB8[i][j]>255?255:resultB8[i][j]):0;
            }
        }
        int tempR[8] = {0,0,0,0,0,0,0,0};
        int tempG[8] = {0,0,0,0,0,0,0,0};
        int tempB[8] = {0,0,0,0,0,0,0,0};
        for(int i = 1;i < newHeight-1;++i)
        {
            for(int j =1;j< newWidth-1;++j)
            {
                tmpR[i][j] = 0;
                tmpG[i][j] = 0;
                tmpB[i][j] = 0;

                tempR[0] = resultR1[i][j];
                tempR[1] = resultR2[i][j];
                tempR[2] = resultR3[i][j];
                tempR[3] = resultR4[i][j];
                tempR[4] = resultR5[i][j];
                tempR[5] = resultR6[i][j];
                tempR[6] = resultR7[i][j];
                tempR[7] = resultR8[i][j];

                tempG[0] = resultG1[i][j];
                tempG[1] = resultG2[i][j];
                tempG[2] = resultG3[i][j];
                tempG[3] = resultG4[i][j];
                tempG[4] = resultG5[i][j];
                tempG[5] = resultG6[i][j];
                tempG[6] = resultG7[i][j];
                tempG[7] = resultG8[i][j];

                tempB[0] = resultB1[i][j];
                tempB[1] = resultB2[i][j];
                tempB[2] = resultB3[i][j];
                tempB[3] = resultB4[i][j];
                tempB[4] = resultB5[i][j];
                tempB[5] = resultB6[i][j];
                tempB[6] = resultB7[i][j];
                tempB[7] = resultB8[i][j];

                for(int k = 0;k<8;k++){
                tmpR[i][j] = tmpR[i][j] > tempR[k]? tmpR[i][j] : tempR[k];
                tmpG[i][j] = tmpG[i][j] > tempG[k]? tmpG[i][j] : tempG[k];
                tmpB[i][j] = tmpB[i][j] > tempB[k]? tmpB[i][j] : tempB[k];
                }
            }
            ImagTranslate::vector2ColorImage(tmpR,tmpG,tmpB,*newImg);
        }
        //将tmpR,tmpG,tmpB转回图像中
        delete img;
        img = NULL;
        return newImg;
}

QImage *ImageSegmentation::CustomEdge(QImage *img,int * selfTemplate)
{
     QImage * newImg = new QImage(*img);
    int newHeight = newImg->height(),
            newWidth = newImg->width();
    vector<vector<float>> tmpR;
    vector<vector<float>> tmpG;
    vector<vector<float>> tmpB;
    ImagTranslate::colorImage2Vector(*img,tmpR,tmpG,tmpB);
    vector<vector<float>> resultR = tmpR;
    vector<vector<float>> resultG = tmpG;
    vector<vector<float>> resultB = tmpB;
    /*
     * selfTemplate 的顺序
     *  0  1  2
     *  3  4  5
     *  6  7  8
     *
     */
    for(int i = 1;i < newHeight-1;++i)
    {
        for(int j =1;j< newWidth-1;++j)
        {
            for(int k =0;k<9;++k)
            {
                resultR[i][j] = tmpR[i-1][j-1] * selfTemplate[0]    + tmpR[i-1][j] * selfTemplate[1] + tmpR[i-1][j+1] * selfTemplate[2]
                              + tmpR[i][j-1] * selfTemplate[3]      + tmpR[i][j] * selfTemplate[4]   + tmpR[i][j+1] * selfTemplate[5]
                              + tmpR[i+1][j-1] * selfTemplate[6]    + tmpR[i+1][j] * selfTemplate[7] + tmpR[i+1][j+1] * selfTemplate[8];
                resultG[i][j] = tmpG[i-1][j-1] * selfTemplate[0]    + tmpG[i-1][j] * selfTemplate[1] + tmpG[i-1][j+1] * selfTemplate[2]
                              + tmpG[i][j-1] * selfTemplate[3]      + tmpG[i][j] * selfTemplate[4]   + tmpG[i][j+1] * selfTemplate[5]
                              + tmpG[i+1][j-1] * selfTemplate[6]    + tmpG[i+1][j] * selfTemplate[7] + tmpG[i+1][j+1] * selfTemplate[8];
                resultB[i][j] = tmpB[i-1][j-1] * selfTemplate[0]    + tmpB[i-1][j] * selfTemplate[1] + tmpB[i-1][j+1] * selfTemplate[2]
                              + tmpB[i][j-1] * selfTemplate[3]      + tmpB[i][j] * selfTemplate[4]   + tmpB[i][j+1] * selfTemplate[5]
                              + tmpB[i+1][j-1] * selfTemplate[6]    + tmpB[i+1][j] * selfTemplate[7] + tmpB[i+1][j+1] * selfTemplate[8];

            }

            resultR[i][j] = resultR[i][j]>0?(resultR[i][j]>255?255:resultR[i][j]):0;
            resultG[i][j] = resultG[i][j]>0?(resultG[i][j]>255?255:resultG[i][j]):0;
            resultB[i][j] = resultB[i][j]>0?(resultB[i][j]>255?255:resultB[i][j]):0;
        }
    }
    //转回图像
     ImagTranslate::vector2ColorImage(resultR,resultG,resultB,*newImg);
     delete img;
     img = NULL;
     return newImg;
}

//这里img默认输入为二值化后的图片
//size为gauss平滑的时候输入矩阵大小，t 为高斯平滑的标准差
//Ostu表示外部获得的大津阈值
//rat表示低阈值获取时高阈值所乘的系数因子
//实现过程中使用的算法都是较为简单处理的版本
QImage *ImageSegmentation::ContourExtraction(QImage *img,int size,int t ,int Ostu,float rat)
{
    QImage * newImg = ImageEnhancement::GaussianSmoothing(img,size,t);
   int newHeight = newImg->height(),
           newWidth = newImg->width();
   vector<vector<float>> tmp;
   ImagTranslate::greyImage2Vector(*newImg,tmp);
   //theta用于存储各点的梯度方向角
   vector<vector<float> > theta = tmp;
   //MRange用于梯度计算
   vector<vector<float> > MRange = tmp;
   //   上面直接赋值为tmp是为了避免计算最边缘的一个像素
   float resultX,resultY;
   for(int i = 1;i < newHeight-1;++i)
   {
       for(int j =1;j< newWidth-1;++j)
       {
               resultX      = tmp[i-1][j+1] + 2*tmp[i][j+1] + tmp[i+1][j+1]-(tmp[i-1][j-1] + 2*tmp[i][j-1] + tmp[i+1][j-1]);
               resultY      = tmp[i+1][j+1] + 2*tmp[i+1][j] + tmp[i+1][j-1]-(tmp[i-1][j+1] + 2*tmp[i-1][j] + tmp[i-1][j-1]);
               //               这里resultX和resultY的方向有可能放反了,前面求横向梯度和纵向梯度可能都求反了，不过不影响前面的结果
               //               求梯度方向角，并转换成角度制
               MRange[i][j] = sqrt( resultX* resultX+ resultY* resultY);
               theta[i][j]  = atan2(resultY,resultX)*57.3;
               int cesi1 = theta[i][j];
               //小于0度转换为反向角度值
               theta[i][j]  = theta[i][j] >0? theta[i][j] : theta[i][j] + 180;
               cesi1 = theta[i][j];
               if(theta[i][j]>22.5&&theta[i][j]<77.5)
               {
                   //这里认为1表示45°
                   theta[i][j] = 1;
                   cesi1 = theta[i][j];
               }
               else if(theta[i][j]>=77.5&&theta[i][j]<112.5)
               {
                   //这里认为2表示90°
                   theta[i][j] = 2;
                   cesi1 = theta[i][j];
               }
               else if(theta[i][j]>=112.5&&theta[i][j]<157.5)
               {
                   //这里认为3表示135°
                   theta[i][j] = 3;
                   cesi1 = theta[i][j];
               }
               else
               {
                   //                   这里认为0代表水平方向
                   theta[i][j] = 0;
                   cesi1 = theta[i][j];
               }
       }
   }
   //   非极大值抑制
   vector<vector<float>> NRange = MRange;
   float HighThreshold = Ostu;
   float LowThreshold = Ostu*rat;
   //下面有两种处理方式
   //   第一种直接将该点像素与梯度方向上的相邻像素比较，之后进行滞后阈值处理
   //   第二种是通过计算四个点之间的线性插值

   //这里采用第一种处理方式
   for(int i = 1;i < newHeight-1;++i)
   {
       for(int j =1;j< newWidth-1;++j)
       {
           //下面非极大值抑制的时候 可能不需要等号
           switch ((int)theta[i][j]) {
           case 0:
                if(MRange[i][j]>=MRange[i][j-1]&&MRange[i][j]>=MRange[i][j+1])
                {
                    NRange[i][j] = MRange[i][j];
                }
                else
                {
                    NRange[i][j] = 0;
                }
               break;
           case 1:
               if(MRange[i][j]>=MRange[i+1][j-1]&&MRange[i][j]>=MRange[i-1][j+1])
               {
                   NRange[i][j] = MRange[i][j];
               }
               else
               {
                   NRange[i][j] = 0;
               }
               break;
           case 2:
               if(MRange[i][j]>=MRange[i-1][j]&&MRange[i][j]>=MRange[i+1][j])
               {
                   NRange[i][j] = MRange[i][j];
               }
               else
               {
                   NRange[i][j] = 0;
               }
               break;
           case 3:
               if(MRange[i][j]>=MRange[i-1][j-1]&&MRange[i][j]>=MRange[i+1][j+1])
               {
                   NRange[i][j] = MRange[i][j];
               }
               else
               {
                   NRange[i][j] = 0;
               }
               break;
           }

       }
   }
   // 因为使用sobel算子求梯度   处理完了对图片最外层一个像素全部置零处理

   //for(){
   //}

   //   再进行双阈值分割
   //   这里双阈值阈值的选取有两种取法
   //   一通过统计直方图来取
   //   二通过大津阈值分割来取得高阈值

   //   这里选用外部传递过来的大津阈值

   QSize * sz  = new  QSize ;
   *sz = newImg->size();
   vector<vector<float>> result = NRange;
   for(int i = 1;i < newHeight-1;++i)
   {
       for(int j =1;j< newWidth-1;++j)
       {
           //这里将结果与高阈值比较，若大于则为强边界
           if(NRange[i][j] >= HighThreshold)
           {
                result[i][j] = 255;
                //i表示深度
                TraceEdge(i,j,LowThreshold,result,NRange,sz);
           }
       }
   }

   for(int i = 0;i < newHeight;++i)
   {
       for(int j =0;j< newWidth;++j)
       {
            if(result[i][j] != 255)
            {
                result[i][j] = 0;
            }
       }
   }
   //转回图像


    ImagTranslate::vector2GreyImage(result,*newImg);
    return newImg;
}

void ImageSegmentation::TraceEdge(int y, int x, float nThrLow, vector<vector<float>> &res, vector<vector<float>> &pMag, QSize *sz)
{
    //对8邻域像素进行查询
    int xNum[8] = {1,1,0,-1,-1,-1,0,1};
    int yNum[8] = {0,1,1,1,0,-1,-1,-1};
    int yy,xx,k;
    for(k=0;k<8;k++)
    {
        yy = y+yNum[k];
        xx = x+xNum[k];
        if(yy<sz->rheight()&&xx<sz->rwidth()&&yy>=0&&xx>=0){
            if(pMag[yy][xx]>=nThrLow )
            {
                //该点设为边界点
                res[yy][xx] =255;
                //以该点为中心再进行跟踪
                TraceEdge(yy,xx,nThrLow,res,pMag,sz);
            }
        }
        else
        {
         return;
        }

    }
}


