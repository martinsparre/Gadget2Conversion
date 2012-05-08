Gadget2Conversion
=================

Convert between ascii and gadget2 files.



How to compile the programs 
=================

    gcc AsciiToGadget.c -o AsciiToGadget
    gcc GadgetToAscii.c -o GadgetToAscii

How to use them 
=================

Create an ascii file:

    $./GadgetToAscii Hernquist100000_000 Hernquist100000_000.txt

Convert back to Gadget2:

    $./AsciiToGadget Hernquist100000_000.txt NewGadget.bin

Convert back to text

    $./GadgetToAscii NewGadget.bin NewText.txt

Check that the two text-files are identical:

    $diff Hernquist100000_000.txt NewText.txt


The Ascii format 
=================

First the number of particles are written. Then "x y z vx vy vz m ID V" for each particle. An Example

    $head NewText.txt 
    100000
    -5.31154108	-5.7146945	-5.499882698	0.05753127486	0.005806070752	0.05357303098	8.263792552E-06	83417	-0.08647640795
    -5.113169193	-5.289948463	-5.168134689	-0.04419142008	-0.07203647494	0.01301814336	8.263792552E-06	45946	-0.09168834239
    -5.907653332	-5.305150986	-5.397208214	0.07210867852	0.02624133416	-0.01221998222	8.263792552E-06	47645	-0.08598073572
    -6.711863041	-5.277193069	-5.176847458	0.001581577351	0.001462606364	0.008624169976	8.263792552E-06	58559	-0.08269295096
    -7.822186947	-5.672159195	-0.8779277205	0.003446541261	-0.05692540109	0.02024648525	8.263792552E-06	39758	-0.08505904675
    -7.84139204	-4.968189716	-3.299323559	-0.03282074258	-0.004592051264	0.02146905661	8.263792552E-06	81970	-0.08381415159
    -6.434488773	-5.248922348	-3.239396095	0.07056804746	0.04399776086	-0.02401136421	8.263792552E-06	97971	-0.09243297577
    -6.55498457	-5.00270319	-3.456570148	0.04789669439	-0.02548005432	-0.03845736384	8.263792552E-06	82419	-0.09214800596
    -7.295172691	-5.154963017	-3.635895729	0.03139507398	-0.04554229975	0.02069512941	8.263792552E-06	20205	-0.08556041121

Regarding potentials: When Running AsciiToGadget the potential (V) of each particle has to be set. The value of the potential is not used for anything by Gadget, so an arbitrary value can be set.

