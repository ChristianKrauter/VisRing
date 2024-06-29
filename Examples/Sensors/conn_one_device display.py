from bleak import BleakClient, discover, BleakError
import asyncio
import struct
from functools import partial
import time
import bleak
from datetime import datetime
import keyboard
import warnings
import visualize_data
import read_file_name
import json
import shutil
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading
import numpy as np
import os

warnings.filterwarnings("ignore", category=DeprecationWarning)
count = 0

UART_RX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e" #Nordic NUS characteristic for RX

# BLE Mac
peri1_mac = ['PPG_Ring#1', 'E932ACBD-9436-4CCC-A3D9-CB87A2093DCB']
peri2_mac = ['PPG_Ring#2', '4AD2D878-9EFB-4619-953A-CF0E226BD799']

time_latency = 50
data_size = 15
data_display = [0,1,2,13]
total_display = 500
total_y_data = [[] for _ in range(data_size)]
ble_conncted = False

display_loop_finished = False
tmp_add = [peri1_mac,peri2_mac]


start_time = time.time()
end_time = time.time()

def unpack_f_bytearray(bytearray):
    f_data = struct.unpack('f', bytearray)
    return f_data[0]

def unpack_l_bytearray(bytearray):
    l_data = struct.unpack('l', bytearray)
    return l_data[0]

def decode_byte_data(bytedata):
    float_array = []
    for i in range(int(len(bytedata)/4)):

        tmp_float = unpack_f_bytearray(bytedata[i*4:i*4+4])
        float_array.append(tmp_float)
    return float_array

def callback(client: BleakClient, file, sender,data):
    global start_time
    global end_time
    global count
    count += 1
    if count == 1:
        start_time = time.time()
    else:
        end_time = time.time()
    timestamp = time.time()
    result = decode_byte_data(data) + [timestamp]

    update_plot_data(result)

    file.write(str(result)[1:-1] + "\n")

def update_plot_data(new_data):
    global y_data
    global total_y_data

    # for ind_line in range(data_size):
    #     total_y_data[:-1,ind_line] = total_y_data[1:,ind_line]
    #     total_y_data[-1,ind_line] = new_data[ind_line]

    for ind_line in range(data_size):
        total_y_data[ind_line].append(new_data[ind_line])
        # print(total_y_data)
        if len(total_y_data[ind_line]) > total_display:
            total_y_data[ind_line] = total_y_data[ind_line][-total_display:]


def update_plot(i):
    global display_loop_finished
    global ble_conncted
    global count
    global start_time
    global end_time
    tmp_suptitle = 'Time Left: '+str(time_latency-int(end_time-start_time))
    fig.suptitle(tmp_suptitle)
    if ble_conncted:
        for asx_ind, ind_line in enumerate(data_display):
            # print(ind_line,":",total_y_data[ind_line])
            axs[asx_ind].cla()
            # axs[asx_ind].plot(total_y_data[:, ind_line], color="blue", alpha=0.9)
            axs[asx_ind].plot(total_y_data[ind_line], color="blue", alpha=0.9)




    if display_loop_finished:
        plt.close()
        return None


def disconnect_callback(client):
    print("Client with address {} got disconnected!".format(client.address))

def run_connect(addresses,file,latency):

    # loop = asyncio.get_event_loop()

    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)


    tasks = asyncio.gather(
        *(connect_to_device_latency(address, loop, file,latency) for address in addresses)
        # * (connect_to_device_keyboard(address, loop, file) for address in addresses)
    )

    loop.run_until_complete(tasks)

def find_address_name(address):
    for add in tmp_add:
        if address == add[1]:
            return add[0]

async def connect_to_device_latency(address, loop, file,latency):
    global  ble_conncted

    async with BleakClient(address, loop=loop) as client:

        print("connect to ", find_address_name(address))
        ble_conncted = True
        try:
            await client.start_notify(UART_RX_UUID, partial(callback, client,file))
            await asyncio.sleep(latency)  # second
            # while True:
            #     if keyboard.is_pressed("q"):
            #         print("STOP connect to ", find_address_name(address))
            #         break
            await client.stop_notify(UART_RX_UUID)
            # print("end")



        except Exception as e:
            print(e)

async def connect_to_device_keyboard(address, loop, file):
    async with BleakClient(address, loop=loop) as client:

        print("connect to ", find_address_name(address))
        try:
            await client.start_notify(UART_RX_UUID, partial(callback, client,file))
            # await asyncio.sleep(10)  # second
            while True:
                await asyncio.sleep(.1)
                if keyboard.is_pressed("q"):
                    print("STOP connect to ", find_address_name(address))
                    break
            await client.stop_notify(UART_RX_UUID)
            # print("end")


        except Exception as e:
            print(e)

def run_peripheral(imu,latency):
    global display_loop_finished
    f = open(imu["path"], 'w', newline='')

    run_connect([imu["address"]],f,latency)
    f.close()


    diff_time = end_time-start_time
    imu["duration"] = diff_time
    imu["total"] = count - 1
    imu["fs"] = float(count/diff_time)
    imu["stime"] = start_time
    imu["etime"] = end_time

    print(imu)
    file_list = read_file_name.get_list_of_file_name_one_device(parent_path, imu["user_name"], imu["stime"])

    for i in file_list:
        print(i)

    # print(file_list)

    save_data(imu, file_list)
    display_loop_finished = True
    # data = visualize_data.read_file(imu["path"])
    # visualize_data.plot_file(data)
    return imu

def save_dict_to_file(file_path,data):
    with open(file_path, 'w') as convert_file:
        convert_file.write(json.dumps(data))
    # print(file_path," is saved!")

def save_data(imu_info,file_list):

    shutil.copyfile(imu_info["path"], file_list[0])

    tmp_imu_data_dic = {}
    imu_info["path"] = file_list[0]
    # print(imu_info["path"] )

    dict_file_path = file_list[1]
    tmp_imu_data_dic[imu_info["name"]] = imu_info
    save_dict_to_file(dict_file_path, tmp_imu_data_dic)


def get_num_folder(parent_path,user):
    # Get the list of all files and directories

    path = parent_path+"/"+user+"/data"
    if not os.path.exists(path):
        os.makedirs(path)
    dir_list = os.listdir(path)
    print(len(dir_list)+2)

if __name__ == "__main__":
    parent_path = "/Users/taitinglu/Documents/GitHub/ECG-Earphone/User Study"
    # parent_path = "C:/Users/Taiting/Documents/GitHub/ECG-Earphone/User Study"
    imu = {"name": "PPG_Ring#1","user_name":"shawn","address":'8F7AAFFC-5348-4766-A4B2-B7639FF83D1D',"path":"test.txt"}
    # imu = {"name": "PPG_Ring#1", "user_name": "shawn", "address": 'EA:01:4F:EA:09:0B',"path": "test.txt"}
    imu = {"name": "PPG_Ring#1", "user_name": "demo", "address": '89E26766-6317-41A8-9856-7C676E9A657B', "path": "test.txt"}
    imu = {"name": "PPG_Ring#1", "user_name": "demo", "address": 'BCA54F45-F40D-49AE-8E2F-0296CA836428',"path": "test.txt"}
    get_num_folder(parent_path, imu['user_name'])
    matplotlib.use("TkAgg")
    fig, axs = plt.subplots(len(data_display),figsize=(8, 6))

    # imu_info = run_peripheral(imu,10)
    # file_list = read_file_name.get_list_of_file_name_one_device(parent_path, imu_info["user_name"], imu_info["stime"])
    #
    # for i in file_list:
    #     print(i)
    #
    # print(file_list)
    # print(imu_info)
    # save_data(imu_info, file_list)
    #
    # data = visualize_data.read_file(imu_info["path"])
    # visualize_data.plot_file(data)

    data_thread = threading.Thread(target=run_peripheral,args=(imu,time_latency))
    # data_thread.daemon = True  # Daemonize the thread so it exits when the main program exits
    data_thread.start()

    ani = FuncAnimation(fig, update_plot, interval=10)

    plt.show()

    get_num_folder(parent_path, imu['user_name'])