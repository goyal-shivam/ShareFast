cp /home/shivam/code/ShareFast/ShareFast/s-sent_rcvd_file.c /home/shivam/code/ShareFast/Kali_shared/
cp /home/shivam/code/ShareFast/ShareFast/header.h /home/shivam/code/ShareFast/Kali_shared/

printf ">>> copied code to Kali Linux\n"
printf "\n>>> gcc s-sent_rcvd_file.c\n"
gcc s-sent_rcvd_file.c
printf "\n>>> sudo ./a.out\n"
sudo ./a.out

# printf "\n>>> cat host.txt\n"
# cat vm.txt
# printf "\n>>> ls\n"
# ls
# printf "\nconfirmation to remove host.txt - press Ctrl + C to abort\n"
# read ok
# printf "\n>>> rm vm.txt\n"
# rm vm.txt

printf "\n>>> ls\n"
ls


printf "\npython3 check.py\n"
python3 check.py
