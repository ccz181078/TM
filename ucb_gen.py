Ts = [10000000]
for maxT in Ts:
    for LRUH in [1,2,3,4,6,8,12,16]:
        for H in [0,1,2]:
            for tH in [0,1,2]:
                if H+tH+1>=LRUH: continue
                for n in range(1,24+1):
                    print('Action 0 0 0')
                    print(f'FAR CPS_LRU maxT {maxT} LRUH {LRUH} H {H} tH {tH} n {n} run')

for maxT in Ts:
    for H in [1,2,3,4,6,8,12,16]:
        for mod in [1,2,3,4,5,6]:
            for n in range(1,24+1):
                print('Action 0 0 0')
                print(f'FAR RWL_mod maxT {maxT} H {H} mod {mod} n {n} run')

