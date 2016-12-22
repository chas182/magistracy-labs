dd of=big.out if=/dev/zero bs=1M count=512 
mke2fs -c big.out -t ext2
sudo mkdir /mnt/bigfs
sudo mount -o loop big.out /mnt/bigfs
sudo mkdir /mnt/bigfs/first
sudo touch /mnt/bigfs/lab
sudo touch /mnt/bigfs/first/lab-1
