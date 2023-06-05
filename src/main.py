from pytube import YouTube
from moviepy.editor import *
import os
import sys


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
    

link = str(sys.argv[1])
songid = str(sys.argv[2])

print("yt link: " + link)
print("replacement id: " + songid)

convert(

    getVid(link),
    songid

)














