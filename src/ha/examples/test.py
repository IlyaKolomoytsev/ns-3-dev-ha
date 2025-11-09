values = []
values.append((1, 5))
values.append((2, 6))
values.append((5, 7))

start_time = None
last_time = None
last_snr = None
snr_list = []
discret_sum = 0
mean_snr = 0
for currentTime, currentSnr in values:
    if start_time is None:
        start_time = currentTime
        mean_snr = currentSnr
    else:
        discret_sum += (last_snr) * (currentTime - last_time)
        mean_snr = (discret_sum) / (currentTime - start_time)

    last_time = currentTime
    last_snr = currentSnr
    print("mean snr:", mean_snr)
