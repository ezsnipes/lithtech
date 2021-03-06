#!/usr/bin/python3

import sys

logs = []
with open(sys.argv[1], 'r') as log:
    for l in log.readlines():
        if l[:-1] in logs:
            continue
        logs.append(l[:-1])

counts = {
    'Pass': 0,
    'Fail': 0,
    'warning': 0,
    'undefined reference': 0
}
mins = {
    'Pass': int(sys.argv[2]),
    'Fail': int(sys.argv[3]),
    'warning': int(sys.argv[4]),
    'undefined reference': int(sys.argv[5])
}
errs = []
warns = []
for l in logs:
    if 'warning:' in l:
        warns.append(l)
    if 'error:' in l:
        errs.append(l)
    for k in counts.keys():
        if k in l:
            counts[k] += 1

if 'Fail EXE_Lithtech' in logs:
    print('\n'.join(warns))

fmt = 'Pass:  {Pass}\nFail:  {Fail}\n'
fmt += "Warn:  {warning}\nUnDef: {undefined reference}"
print(fmt.format(**counts))
print('warns {0} vs counts {1}'.format(len(warns), counts['warning']))
if mins['Pass'] > counts['Pass']:
    print('Pass parts have gone below known limits')
    print('\n'.join(errs))
    sys.exit(1)

for k in mins.keys():
    if 'Pass' in k:
        continue
    if mins[k] < counts[k]:
        if 'warn' in k and mins[k] < len(warns):
            continue
        print('{0} parts have gone above known limits'.format(k))
        sys.exit(1)
