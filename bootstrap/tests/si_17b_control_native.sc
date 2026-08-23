/* si_17b_control_native.sc — native counterpart: same behavior as sc4_control.sc. */
OUTPUT = '--- interp ---';
x = 10;
if (GT(x, 5)) OUTPUT = 'big'; else OUTPUT = 'small';
if (LT(x, 5)) OUTPUT = 'small'; else OUTPUT = 'big';
y = 3;
if (EQ(y, 3)) OUTPUT = 'three';
if (EQ(y, 4)) OUTPUT = 'four'; else OUTPUT = 'not four';
OUTPUT = '--- done ---';
