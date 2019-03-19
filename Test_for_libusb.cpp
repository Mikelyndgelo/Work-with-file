#include "pch.h"
// ConsoleApplication5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include<iostream>
#include<libusb-1.0/libusb.h>
#include<map>
#include<regex>

using namespace std;



/*void printdev(libusb_device *dev); //prototype of the function
int main() {

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices

	libusb_context *ctx = NULL; //a libusb session

	int r; //for return values

	ssize_t cnt; //holding number of devices in list

	r = libusb_init(&ctx); //initialize a library session

	if (r < 0) {

		cout << "Init Error " << r << endl; //there was an error

		return 1;

	}

	libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation

	cnt = libusb_get_device_list(ctx, &devs); //get the list of devices

	if (cnt < 0) {

		cout << "Get Device Error" << endl; //there was an error

	}

	cout << cnt << " Devices in list." << endl; //print total number of usb devices

	ssize_t i; //for iterating through the list

	for (i = 0; i < cnt; i++) {

		printdev(devs[i]); //print specs of this device

	}

	libusb_free_device_list(devs, 1); //free the list, unref the devices in it

	libusb_exit(ctx); //close the session

	return 0;

}



void printdev(libusb_device *dev) {

	libusb_device_descriptor desc;

	int r = libusb_get_device_descriptor(dev, &desc);

	if (r < 0) {

		cout << "failed to get device descriptor" << endl;

		return;

	}
	cout << "Number of possible configurations: " << (int)desc.bNumConfigurations << "  ";

	cout << "Device Class: " << (int)desc.bDeviceClass << "  ";

	cout << "VendorID: " << desc.idVendor << "  ";

	cout << "ProductID: " << desc.idProduct << endl;

	libusb_config_descriptor *config;

	libusb_get_config_descriptor(dev, 0, &config);

	cout << "Interfaces: " << (int)config->bNumInterfaces << " ||| ";

	const libusb_interface *inter;

	const libusb_interface_descriptor *interdesc;

	const libusb_endpoint_descriptor *epdesc;

	for (int i = 0; i < (int)config->bNumInterfaces; i++) {

		inter = &config->interface[i];

		cout << "Number of alternate settings: " << inter->num_altsetting << " | ";

		for (int j = 0; j < inter->num_altsetting; j++) {

			interdesc = &inter->altsetting[j];

			cout << "Interface Number: " << (int)interdesc->bInterfaceNumber << " | ";

			cout << "Number of endpoints: " << (int)interdesc->bNumEndpoints << " | ";

			for (int k = 0; k < (int)interdesc->bNumEndpoints; k++) {

				epdesc = &interdesc->endpoint[k];

				cout << "Descriptor Type: " << (int)epdesc->bDescriptorType << " | ";

				cout << "EP Address: " << (int)epdesc->bEndpointAddress << " | ";

			}

		}

	}

	cout << endl << endl << endl;

	libusb_free_config_descriptor(config);

}*/

int main()
{
	std::string str = "Hello world";
	std::tr1::regex rx("ello");
	regex_match(str.begin(), str.end(), rx);
	regex_search(str.begin(), str.end(), rx);

	
	map<int, int> mapc;
	mapc.insert(546, 456);
	mapc.insert(77, 66);
	for (auto el : mapc)
	{
		
	}
	return 0;


}
#include "VSQWebSwitcher.h"

#include <QDesktopWidget>
#include <QtWidgets/QApplication>
#include <QtCore/QTimer>
#include<libusb-1.0/libusb.h>

//
#include<QRegExp>
#include<QTextCodec>	//convert from QByteArray to QString
#include<QFile>



#include <QObject>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QString>
#include <QSettings>
#include<QMap>

class QDesktopWidget;

//Наши девайсы(MIMO) Vendor/ProductID 6720/1025, 6121/363, 9838/256 ????

class VSQWebSwitcher : public QObject
{
	Q_OBJECT
		QWebEngineView * m_widget;
	QString m_url;
	int m_screenNumber;
	QSettings m_settings;
	QMap<int, int> Vendor_ProductID_from_device;
	QMap<int, int>Vendor_ProductID_from_File;
	QDesktopWidget *desktop;

public:
	VSQWebSwitcher(QWebEngineView * w);
	~VSQWebSwitcher();
	bool Need_Make_PrimaryScreen();
	void onDisplayAvailable(bool avail);
public slots:
	void GetVendor_ProductID();
protected:
	void showWidget();
};

void VSQWebSwitcher::GetVendor_ProductID()
{
	libusb_context *context = nullptr;
	libusb_device **list = nullptr;

	libusb_init(&context);
	int count = libusb_get_device_list(context, &list);

	for (size_t idx = 0; idx < count; ++idx)
	{
		libusb_device *device = list[idx];
		libusb_device_descriptor desc = { 0 };

		libusb_get_device_descriptor(device, &desc);

		int key = desc.idVendor;
		int value = desc.idProduct;
		Vendor_ProductID_from_device.insert(key, value);
	}
}

//Функция должна считывать информацию из файла VendorID и ProductID и сравнивать его с имеющимися идентификаторам
bool VSQWebSwitcher::Need_Make_PrimaryScreen()
{
	QFile VendorID_ProductID("C:\Repositories\QtClients\DisplayWatcher\VendorID_ProductID.txt");

	//VendorID_ProductID.open(QIODevice::ReadOnly);
	if (VendorID_ProductID.open(QIODevice::WriteOnly))
	{
		//VendorID_ProductID.write("Test string123\n");
		//VendorID_ProductID.write("Test string2");
		VendorID_ProductID.close();
	}
	QString str = "";
	if ((VendorID_ProductID.exists()) && (VendorID_ProductID.open(QIODevice::ReadOnly)))
	{
		//str = "";
		while (!VendorID_ProductID.atEnd())
		{
			str = str + VendorID_ProductID.readLine();
		}
		VendorID_ProductID.close();
	}

	//QString::fromStdString(text.toStdString());
	//QString str = QTextCodec::codecForMib(1015)->toUnicode(text);		//need convert
	//QString str = text;
	int size = str.size();
	QString temp_str;

	QRegExp regexp("[0-9]");
	QString key, value;
	/*for (int i = 0; i < str.size(); i++)
	{
		temp_str = str[i];
		key = "";
		value = "";
		if (temp_str.contains(regexp) > 0)
		{
			while (str[i] != '\n' || str[i] != '-' ||  i < str.size() || str[i] != ' ')
			{
				key += str[i];
				i++;
			}
			while (str[i] != '\n' || str[i] != '-' || i < str.size() || str[i] != ' ')
			{
				value += str[i];
				i++;
			}

		}
		Vendor_ProductID_from_File.insert(key.toInt(), value.toInt());
	}*/

	for (auto it_device = Vendor_ProductID_from_device.begin(); it_device != Vendor_ProductID_from_device.end(); ++it_device)
	{
		for (auto it_File = Vendor_ProductID_from_File.begin(); it_File != Vendor_ProductID_from_File.end(); ++it_File)
		{
			if (it_File.key() == it_device.key() && it_File.value() == it_device.value())
			{
				return true;
			}
		}
	}
	return false;
}
