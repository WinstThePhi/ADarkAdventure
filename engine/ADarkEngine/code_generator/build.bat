@echo off

cl %warnings% %defaultCompilerFlags%  ..\engine\ADarkEngine\code_generator\code_generator.c 

cl %warnings% %defaultCompilerFlags% ..\engine\ADarkEngine\code_generator\simple_preprocessor.c 