/*
  modificacion alpha2
*/

#include <iostream>
#include <boost/asio.hpp>
#include "vmu931/commands.hpp"
#include "vmu931/sensor.hpp"
#include "vmu931/types.hpp"
#include <boost/date_time.hpp>


boost::posix_time::ptime now = boost::date_time::not_a_date_time;
boost::posix_time::ptime last1 = boost::date_time::not_a_date_time;
boost::posix_time::ptime last2 = boost::date_time::not_a_date_time;
boost::posix_time::ptime last3 = boost::date_time::not_a_date_time;
boost::posix_time::ptime last4 = boost::date_time::not_a_date_time;
boost::posix_time::ptime last5 = boost::date_time::not_a_date_time;



int main()
{
    last2 = boost::posix_time::microsec_clock::universal_time();
    int y = 0;

    boost::asio::io_service io_service;
    boost::asio::serial_port serial_port(io_service, "/dev/ttyACM0");
    //boost::asio::serial_port serial_port(io_service, "/dev/tty.usbmodem931901443E");

    vmu931::Sensor sensor(std::move(serial_port));

    sensor.register_sink([](vmu931::Accelerometers accel) {
      now = boost::posix_time::microsec_clock::universal_time();
/*          std::cout <<" " <<accel.x <<" " <<accel.y <<" " <<accel.z;
      //std::cout <<"Time: " <<now <<" --Dif: " <<now-last1 <<std::endl;
      last1 = now;
*/
      if(now-last1 >= boost::posix_time::milliseconds(1000)) {
          std::cout <<" " <<accel.x <<" " <<accel.y <<" " <<accel.z;
        last1 = now;
      }
    });

    sensor.register_sink([](vmu931::Gyroscopes gyro) {
      now = boost::posix_time::microsec_clock::universal_time();
/*          std::cout <<" " <<gyro.x <<" " <<gyro.y <<" " <<gyro.z;
      //std::cout <<"Time: " <<now <<" --Dif: " <<now-last2 <<std::endl;
      last2 = now;
*/
      if(now-last2 >= boost::posix_time::milliseconds(1000)) {
          std::cout <<" " <<gyro.x <<" " <<gyro.y <<" " <<gyro.z;
        last2 = now;
      }
    });

    sensor.register_sink([](vmu931::Magnetometers magneto) {
        std::cout << "magneto: x=" << magneto.x << " y=" << magneto.y << " z=" << magneto.z <<"\n";
    });

    sensor.register_sink([](vmu931::EulerAngles euler) {
      now = boost::posix_time::microsec_clock::universal_time();
/*        std::cout <<" " <<euler.x <<" " <<euler.y <<" " <<euler.z;
      //std::cout <<"Time: " <<now <<" --Dif: " <<now-last3 <<std::endl;
      last3 = now;
*/
      if(now-last3 >= boost::posix_time::milliseconds(1000)) {
          std::cout <<" " <<euler.x <<" " <<euler.y <<" " <<euler.z;
        last3 = now;
      }
    });

    sensor.register_sink([](vmu931::Quaternions quat) {
      now = boost::posix_time::microsec_clock::universal_time();
/*      auto diff = now-last4;
      std::cout <<now <<" " <<diff.total_microseconds();
        std::cout <<" " <<quat.w <<" " <<quat.x <<" " <<quat.y <<" " <<quat.z;
      last4 = now;
*/
      if(now-last4 >= boost::posix_time::milliseconds(1000)) {
        auto diff = now-last4;
        std::cout <<now <<" " <<diff.total_microseconds();
          std::cout <<" " <<quat.w <<" " <<quat.x <<" " <<quat.y <<" " <<quat.z;
        last4 = now;
      }
    });

    sensor.register_sink([](vmu931::Heading h) {
      now = boost::posix_time::microsec_clock::universal_time();
/*        std::cout <<" " <<h.heading <<std::endl;
      //std::cout <<"Time: " <<now <<" --Dif: " <<now-last5 <<std::endl;
      last5 = now;
      //std::cout <<std::endl;
*/
      if(now-last5 >= boost::posix_time::milliseconds(1000)) {
          std::cout <<" " <<h.heading <<std::endl;
        last5 = now;
      }
    });

    sensor.register_sink([](std::string s) {
        std::cout << "message: " << s << "\n";
    });
    sensor.register_sink([](vmu931::Status status) {
        std::cout << "VMU931 status:\n"
            << " - streams: " << status.streaming() << "\n"
            << " - gyroscopes: " << status.resolution_gyroscopes() << " dps\n"
            << " - accelerometers: " << status.resolution_accelerometers() << " g\n"
            << " - output rate: " << status.output_rate() << " Hz\n";
    });

    io_service.post([&sensor]() {
      sensor.set_streams({
        vmu931::commands::Accelerometers,
        vmu931::commands::Gyroscopes,
        //vmu931::commands::Magnetometers,
        vmu931::commands::EulerAngles,
        vmu931::commands::Quaternions,
        vmu931::commands::Heading,
        //vmu931::commands::Status
      });
    });

    std::cout << "Start reading VMU931 sensor stream...\n";
    std::cout <<"Date Time " <<"Diftime[microsec] " <<"/quatW " <<"quatX " <<"quatY " <<"quatZ " <<"/accelX " <<"accelY " <<"accelZ "
      <<"/gyroX " <<"gyroY " <<"gyroZ " <<"/eulerX "  <<"eulerY " <<"eulerZ " <<"/heading" <<std::endl;
    io_service.run();

  return 0;
}




/*
  lo que nos ha dicho paco en la tutoria, branch paco

#include <iostream>
#include <boost/asio.hpp>
#include "vmu931/commands.hpp"
#include "vmu931/sensor.hpp"
#include "vmu931/types.hpp"
#include <boost/date_time.hpp>


boost::posix_time::ptime now = boost::date_time::not_a_date_time;
boost::posix_time::ptime last = boost::date_time::not_a_date_time;


int main()
{
    last = boost::posix_time::microsec_clock::universal_time();
    boost::asio::io_service io_service;
    boost::asio::serial_port serial_port(io_service, "/dev/ttyACM0");
    //boost::asio::serial_port serial_port(io_service, "/dev/tty.usbmodem931901443E");

    vmu931::Sensor sensor(std::move(serial_port));
    sensor.register_sink([](vmu931::Accelerometers accel) {
        std::cout << "accel: x=" << accel.x << " y=" << accel.y << " z=" << accel.z <<"\n";
    });
    sensor.register_sink([](vmu931::Gyroscopes gyro) {
        std::cout << "gyro: x=" << gyro.x << " y=" << gyro.y << " z=" << gyro.z <<"\n";
    });
    sensor.register_sink([](vmu931::Magnetometers magneto) {
        std::cout << "magneto: x=" << magneto.x << " y=" << magneto.y << " z=" << magneto.z <<"\n";
    });
    sensor.register_sink([](vmu931::EulerAngles euler) {
      now = boost::posix_time::microsec_clock::universal_time();
      //std::cout<<now-last<<std::endl;
      if(now-last >= boost::posix_time::seconds(1.0)) {
        std::cout <<now <<std::endl;
        std::cout << "euler: x=" << euler.x << " y=" << euler.y << " z=" << euler.z <<"\n";
        last = now;
      }
    });
    sensor.register_sink([](vmu931::Quaternions quat) {
        std::cout << "quat: w=" << quat.w << " x=" << quat.x << " y=" << quat.y << " z=" << quat.z <<"\n";
    });
    sensor.register_sink([](vmu931::Heading h) {
      now = boost::posix_time::microsec_clock::universal_time();
      std::cout <<now <<std::endl;
        std::cout << "heading: " << h.heading << "\n";
    });
    sensor.register_sink([](std::string s) {
        std::cout << "message: " << s << "\n";
    });
    sensor.register_sink([](vmu931::Status status) {
        std::cout << "VMU931 status:\n"
            << " - streams: " << status.streaming() << "\n"
            << " - gyroscopes: " << status.resolution_gyroscopes() << " dps\n"
            << " - accelerometers: " << status.resolution_accelerometers() << " g\n"
            << " - output rate: " << status.output_rate() << " Hz\n";
    });

    io_service.post([&sensor]() {
      sensor.set_streams({
        //vmu931::commands::Accelerometers,
        //vmu931::commands::Gyroscopes,
        //vmu931::commands::Magnetometers,
        vmu931::commands::EulerAngles,
        //vmu931::commands::Quaternions,
        //vmu931::commands::Heading,
        //vmu931::commands::Status
      });
    });

    std::cout << "Start reading VMU931 sensor stream...\n";
    io_service.run();
    /*while(true) {
      auto start = boost::posix_time::microsec_clock::universal_time();
      io_service.poll_one();
      auto stop = boost::posix_time::microsec_clock::universal_time();
      auto diff = stop - start;
      auto to_sleep = boost::posix_time::seconds(1.0) - diff;
      usleep(to_sleep.total_microseconds());
    }*
    return 0;
}*/





/*
#include <iostream>
#include <boost/asio.hpp>
#include "vmu931/commands.hpp"
#include "vmu931/sensor.hpp"
#include "vmu931/types.hpp"
#include <boost/date_time.hpp>


boost::posix_time::ptime now = boost::date_time::not_a_date_time;
boost::posix_time::ptime last = boost::date_time::not_a_date_time;


int main()
{
    last = boost::posix_time::microsec_clock::universal_time();
    boost::asio::io_service io_service;
    boost::asio::serial_port serial_port(io_service, "/dev/ttyACM0");
    //boost::asio::serial_port serial_port(io_service, "/dev/tty.usbmodem931901443E");


    vmu931::Sensor sensor(std::move(serial_port));
    sensor.register_sink([](vmu931::Accelerometers accel) {
        std::cout << "accel: x=" << accel.x << " y=" << accel.y << " z=" << accel.z <<"\n";
    });
    sensor.register_sink([](vmu931::Gyroscopes gyro) {
        std::cout << "gyro: x=" << gyro.x << " y=" << gyro.y << " z=" << gyro.z <<"\n";
    });
    sensor.register_sink([](vmu931::Magnetometers magneto) {
        std::cout << "magneto: x=" << magneto.x << " y=" << magneto.y << " z=" << magneto.z <<"\n";
    });
    sensor.register_sink([](vmu931::EulerAngles euler) {
        //std::cout << "euler: x=" << euler.x << " y=" << euler.y << " z=" << euler.z <<"\n";
      now = boost::posix_time::microsec_clock::universal_time();
      //std::cout <<"tiempo: " <<now <<std::endl;
      //std::cout<<now-last<<std::endl;
      if(now-last >= boost::posix_time::seconds(1)) {
        std::cout <<now <<std::endl;
        std::cout << "euler: x=" << euler.x << " y=" << euler.y << " z=" << euler.z <<"\n";
        last = now;
      }
    });
    sensor.register_sink([](vmu931::Quaternions quat) {
        std::cout << "quat: w=" << quat.w << " x=" << quat.x << " y=" << quat.y << " z=" << quat.z <<"\n";
    });
    sensor.register_sink([](vmu931::Heading h) {
      now = boost::posix_time::microsec_clock::universal_time();
      std::cout <<now <<std::endl;
        std::cout << "heading: " << h.heading << "\n";
    });
    sensor.register_sink([](std::string s) {
        std::cout << "message: " << s << "\n";
    });
    sensor.register_sink([](vmu931::Status status) {
        std::cout << "VMU931 status:\n"
            << " - streams: " << status.streaming() << "\n"
            << " - gyroscopes: " << status.resolution_gyroscopes() << " dps\n"
            << " - accelerometers: " << status.resolution_accelerometers() << " g\n"
            << " - output rate: " << status.output_rate() << " Hz\n";
    });

    io_service.post([&sensor]() {
      sensor.set_streams({
        //vmu931::commands::Accelerometers,
        //vmu931::commands::Gyroscopes,
        //vmu931::commands::Magnetometers,
        vmu931::commands::EulerAngles,
        //vmu931::commands::Quaternions,
        //vmu931::commands::Heading,
        //vmu931::commands::Status
      });
    });

    std::cout << "Start reading VMU931 sensor stream...\n";
    io_service.run();
/*    int i=0;
    while(i<5) {
      auto start = boost::posix_time::microsec_clock::universal_time();
        std::cout <<"huam" <<std::endl;
      //io_service.run_one();
      io_service.post([&sensor]() {
            sensor.set_streams({
              //vmu931::commands::Accelerometers,
              //vmu931::commands::Gyroscopes,
              //vmu931::commands::Magnetometers,
              vmu931::commands::EulerAngles,
              //vmu931::commands::Quaternions,
              //vmu931::commands::Heading,
              //vmu931::commands::Status
            });
          });
      io_service.run_one();
      auto stop = boost::posix_time::microsec_clock::universal_time();
      auto diff = stop - start;
      auto to_sleep = boost::posix_time::seconds(1.0) - diff;
        std::cout <<"suuu" <<std::endl;
      usleep(to_sleep.total_microseconds());
        //std::cout <<"SUUUUU" <<std::endl;
        i++;
        std::cout <<i <<std::endl;
    }
    return 0;
}
*/
