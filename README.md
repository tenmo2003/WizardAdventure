# WizardAdventure
This game was made just using SDL2, everything was pretty much hard-coded.
SDL Version used: 32-bit ver.

  -SDL2_image-2.0.5
  
  -SDL2_mixer-2.0.4
  
  -SDL2_ttf-2.0.18
  
  -SDL2-2.0.20

[Details in English below]


Hướng dẫn cài đặt: Cài đặt SDL2 theo hướng dẫn tra qua google tùy IDE. Codeblocks được khuyến nghị do game được code và test trên IDE này. Tiếp theo, tải tất cả mọi file và thư mục res (readme.md optional) trong repo này. Cho các file và res đi chung với nhau như trên repo (tránh bị sai đường dẫn). Tạo project trên IDE và thêm các file cũng như thư mục res vào project đó, build and run sẽ chạy game. (Nếu build & run mà chương trình báo lỗi có chữ .dll trong đó, hãy copy mọi file .dll của SDL vào trong thư mục project (lỗi bước cài đặt))


Mô tả: 
  Game thể loại sống còn. Người chơi cố gắng sống sót lâu nhất có thể. Đến mốc thời gian 10 phút, đợt quái cuối cùng sẽ sinh ra. Tiêu diệt hết tất cả quái để chiến thắng.
  

English:

You should be able to run this well if SDL2 was installed perfectly. If there's any bugs, it should be due to your installation, or your computer isn't quite good, or it is just me forgetting to commit changes.
I personally used CodeBlocks to test and run the game.

Control: WASD for movements, Spacebar for ice-attack. When you hit level 3, you will be able to shoot projectiles, the projectiles will be fired towards your cursor position (keep it inside the window pls). As you progress in level, you will be able to fire more projectiles (spread out) at once.

Objectives: Survive as long as you can! The final wave of enemies comes in at 10 minute mark, if the player manage to clear all the enemies, they win!

As of background music, I added some track*.wav files in the "res" folder. If you want to change the music, you can rename the desired track into "track.wav" (watch out because sometimes windows hide the .wav part and if you change the file name into .wav, its actual name would be track.wav.wav) 
