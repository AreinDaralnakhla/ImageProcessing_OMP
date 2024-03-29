#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <chrono>  // for high_resolution_clock

using namespace std;

void average( const cv::Mat_<cv::Vec3b>& source, cv::Mat_<cv::Vec3b>& destination, int k )
{
  int k2 = k/2;
#pragma omp parallel for
  for (int i=k2;i<source.rows-k2;i++)
    for (int j=k2;j<source.cols-k2;j++)
      for (int c=0;c<3;c++)
        {
          unsigned int temp = 0;
          for (int ii=-k2;ii<=k2;ii++)
            for (int jj=-k2;jj<=k2;jj++)
              temp += source(i+ii,j+jj)[c];

          destination(i,j)[c] = temp/(k*k);
        }
}

int main( int argc, char** argv )
{

  cv::Mat_<cv::Vec3b> source = cv::imread ( argv[1], cv::IMREAD_COLOR);
  cv::Mat_<cv::Vec3b> destination ( source.rows, source.cols );
  cv::imshow("Source Image", source );

  auto begin = chrono::high_resolution_clock::now();
  const int iter = 1;

  int k  = atoi(argv[2]);
  for (int it=0;it<iter;it++)
    {
      average( source, destination, k );
    }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-begin;

  cv::imshow("Processed Image", destination );

  cout << "Total time: " << diff.count() << " s" << endl;
  cout << "Time for 1 iteration: " << diff.count()/iter << " s" << endl;
  cout << "IPS: " << iter/diff.count() << endl;
  
  cv::waitKey();
  return 0;
}

