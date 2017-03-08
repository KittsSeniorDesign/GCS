function funct = datamap(input, in_min, in_max, out_min, out max)
    input = (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
