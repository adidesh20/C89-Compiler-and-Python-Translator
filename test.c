int main()
{
    int x = 55;
    int y = 99;

    int count = 0;

    while(count < 10)
    {
        if(x < y)
        {
            count++;
        }
        else
        {
            return 1;
        }
        
    }

    return 0;
}