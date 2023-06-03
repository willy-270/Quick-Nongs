from pytube import YouTube
from moviepy.editor import *
import os


gdPath = os.getenv('localappdata')+'\\GeometryDash\\'


def getVid(link):
    yt = YouTube(link)

    print("\nTitle: ", yt.title)
    print("\ndownloading...")

    vidLocation = yt.streams.filter(only_audio=True).first().download(gdPath)
    print("Wrote video to: " + vidLocation)

    return vidLocation

def convert(vidLocation, songid):
    mp3_file = gdPath + str(songid) + ".mp3"

    videoclip = AudioFileClip(vidLocation)
    videoclip.write_audiofile(mp3_file)

    videoclip.close()

    os.remove(vidLocation)
    print("Deleted video file.")
    

link = input("Enter link: ")
songid = input("Enter replacement ID: ")

convert(

    getVid(link),
    songid

)














