# WizardAdventure
This game was made just using SDL2, everything was pretty much hard-coded.
SDL Version used: 32-bit ver.

  -SDL2_image-2.0.5
  
  -SDL2_mixer-2.0.4
  
  -SDL2_ttf-2.0.18
  
  -SDL2-2.0.20

[Details in English below]


Hướng dẫn cài đặt: 

Cách 1 (chỉ chạy game): 

   -  Tải MinGW Installer https://sourceforge.net/projects/mingw/. Cài đặt MinGW Installer theo mặc định, đến bước hiện ra các package cài đặt ![image](https://user-images.githubusercontent.com/100185885/169244183-c14b2136-78b2-435f-82ec-7917693660b4.png) Tick chọn mingw32-base và mingw32-gcc-g++, ấn vào Installation -> Apply Changes. Sau khi cài xong, vào thư mục mingw vừa cài đặt, vào bin và copy đường dẫn (VD: C:\MinGW\bin) Add đường dẫn vào PATH trong system variables của environment (có thể google).

   -  Sau đó, tải Repository này xuống qua nút Code màu xanh lá -> download ZIP. Giải nén -> vào thư mục Release -> chạy Project.exe hoặc WizardAdventure.exe để chạy game


Cách 2: Cài đặt SDL2 theo hướng dẫn tra qua google tùy IDE. Codeblocks được khuyến nghị do game được code và test trên IDE này. Tiếp theo, tải tất cả mọi file và thư mục res (readme.md optional) trong repo này. Cho các file và res đi chung với nhau như trên repo (tránh bị sai đường dẫn). Tạo project trên IDE và thêm các file cũng như thư mục res vào project đó, build and run sẽ chạy game. (Nếu build & run mà chương trình báo lỗi có chữ .dll trong đó, hãy copy mọi file .dll của SDL vào trong thư mục project (lỗi bước cài đặt))


Tóm tắt: 
  Game thể loại sống còn. Người chơi cố gắng sống sót lâu nhất có thể trước sự tấn công liên tục từ các đợt quái. Đến mốc thời gian 10 phút, đợt quái cuối cùng sẽ sinh ra. Tiêu diệt hết tất cả quái để chiến thắng.
  
Chức năng:

  -WASD để di chuyển, phím cách(spacebar) để dùng đòn băng, di chuyển chuột điều khiển hướng đạn bắn, đạn sẽ bắn về vị trí con trỏ chuột trong cửa sổ. Khi lên cấp, số đạn sẽ được tăng dần và bắn theo hướng tỏa ra.
  
  -F11: chế độ toàn màn hình, P: dừng trò chơi (khi đang chơi).
  
  -Sẽ có 10 tim được rải ngẫu nhiên quanh map, ăn tim sẽ được hồi 10% máu, sau khi ăn tim, 1 tim mới sẽ được sinh ra theo seed random trên map, luôn có 10 tim tồn tại trên map.

Các kĩ thuật được sử dụng: vector(mảng không dynamic nên không tối ưu cho game), con trỏ và các tham trị, tham chiếu, lớp (cấu trúc tương tự), 4 thư viện SDL2 như đã đề cập, ...
  
  

English:

You should be able to run this well if SDL2 was installed perfectly. If there's any bugs, it should be due to your installation, or your computer isn't quite good, or it is just me forgetting to commit changes.
I personally used CodeBlocks to test and run the game.

Control: WASD for movements, Spacebar for ice-attack. When you hit level 3, you will be able to shoot projectiles, the projectiles will be fired towards your cursor position (keep it inside the window pls). As you progress in level, you will be able to fire more projectiles (spread out) at once. There are also hearts on the map that you can use to heal for 10% of your max health.

Objectives: Survive as long as you can! The final wave of enemies comes in at 10 minute mark, if the player manage to clear all the enemies, they win!

As of background music, I added some track*.wav files in the "res" folder. If you want to change the music, you can rename the desired track into "track.wav" (watch out because sometimes windows hide the .wav part and if you change the file name into .wav, its actual name would be track.wav.wav). Minecraft's track is so good but it's over 25MB so I can't upload it to the res folder.
