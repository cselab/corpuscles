bjobs | awk '{if (NR>1) {print$1}}'  | xargs -n1 bkill
