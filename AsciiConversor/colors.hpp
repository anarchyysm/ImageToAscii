#ifndef COLORS_H
#define COLORS_H
#include <iostream>

namespace hey {
                    
  const std::string black = "\033[1;30m", // normal             
      
                    grayclaro = "\033[90m",

                    gray  = "\033[30;10m", // normal
                    grayn = "\033[30;1m", // negrito
                    grayf = "\033[30;2m", // fraco
                    grayi = "\033[30;3m", // itálico
                    grays = "\033[30;4m", // sublinhado
                    grayp = "\033[30;5m", // piscando
                    grayb = "\033[30;7m", // background
                    grayc = "\033[30;9m", // cancelado

                    red  = "\033[31;10m", // normal
                    redn = "\033[31;1m", // negrito
                    redf = "\033[31;2m", // fraco
                    redi = "\033[31;3m", // itálico
                    reds = "\033[31;4m", // sublinhado
                    redp = "\033[31;5m", // piscando
                    redb = "\033[31;7m", // background
                    redc = "\033[31;9m", // cancelado

                    green  = "\033[32;10m", // normal
                    greenn = "\033[32;1m", // negrito
                    greenf = "\033[32;2m", // fraco
                    greeni = "\033[32;3m", // itálico
                    greens = "\033[32;4m", // sublinhado
                    greenp = "\033[32;5m", // piscando
                    greenb = "\033[32;7m", // background
                    greenc = "\033[32;9m", // cancelado

                    yellow  = "\033[33;10m", // normal
                    yellown = "\033[33;1m", // negrito
                    yellowf = "\033[33;2m", // fraco
                    yellowi = "\033[33;3m", // itálico
                    yellows = "\033[33;4m", // sublinhado
                    yellowp = "\033[33;5m", // piscando
                    yellowb = "\033[33;7m", // background
                    yellowc = "\033[33;9m", // cancelado

                    blue  = "\033[34;10m", // normal
                    bluen = "\033[34;1m", // negrito
                    bluef = "\033[34;2m", // fraco
                    bluei = "\033[34;3m", // itálico
                    blues = "\033[34;4m", // sublinhado
                    bluep = "\033[34;5m", // piscando
                    blueb = "\033[34;7m", // background
                    bluec = "\033[34;9m", // cancelado

                    purple  = "\033[35;10m", // normal
                    purplen = "\033[35;1m", // negrito
                    purplef = "\033[35;2m", // fraco
                    purplei = "\033[35;3m", // itálico
                    purples = "\033[35;4m", // sublinhado
                    purplep = "\033[35;5m", // piscando
                    purpleb = "\033[35;7m", // background
                    purplec = "\033[35;9m", // cancelado

                    cyan  = "\033[36;10m", // normal
                    cyann = "\033[36;1m", // negrito
                    cyanf = "\033[36;2m", // fraco
                    cyani = "\033[36;3m", // itálico
                    cyans = "\033[36;4m", // sublinhado
                    cyanp = "\033[36;5m", // piscando
                    cyanb = "\033[36;7m", // background
                    cyanc = "\033[36;9m", // cancelado

                    white  = "\033[38;10m", // normal
                    whiten = "\033[38;1m", // negrito
                    whitef = "\033[38;2m", // fraco
                    whitei = "\033[38;3m", // itálico
                    whites = "\033[38;4m", // sublinhado
                    whitep = "\033[38;5m", // piscando
                    whiteb = "\033[38;7m", // background
                    whitec = "\033[38;9m", // cancelado

                    off = "\033[m"; // desliga
}

#endif
