
 inline void
DrawPixel(back_buffer* backBuffer, 
          u16 x, u16 y,
          v3 color)
{
    u32* pixel = (u32*)((char*)backBuffer->memory + (x * BYTES_PER_PIXEL) + (backBuffer->pitch * y));
    
    *pixel = color.r << 16 | color.g << 8 | color.b;
}

internal void
DrawRectangle(back_buffer* backBuffer,
              u16 x, u16 y,
              u16 width, u16 height,
              v3 color)
{
    u16 xMin = x;
    u16 yMin = y;
    
    u16 xMax = x + width;
    u16 yMax = y + height;
    
    if(xMin < 0)
        xMin = 0;
    if(yMin < 0)
        yMin = 0;
    if(xMax > backBuffer->width)
        xMax = backBuffer->width;
    if(yMax > backBuffer->height)
        yMax = backBuffer->height;
    
    u8* row = (u8*)((u32*)backBuffer->memory + (y * backBuffer->pitch));
    
    for(u16 yCount = yMin; yCount < yMax; ++yCount)
    {
        u32* pixel = (u32*)row + (xMin * BYTES_PER_PIXEL);
        
        for(u16 xCount = xMin; xCount < xMax; ++xCount)
        {
            *pixel++ = color.r << 16 | color.g << 8 | color.b;
            //DrawPixel(backBuffer, xCount, yCount, color);
        }
        row += backBuffer->pitch;
    }
}

#ifdef TESTING
internal void
DrawLine(back_buffer* backBuffer,
         u16 startX, u16 startY,
         u16 endX, u16 endY,
         v3 color)
{
    u16 changeX = endX - startX;
    u16 changeY = endY - startY;
    
    u16 slope = (u16)abs((f32)changeY / (f32)changeX);
    
    for
}
#endif