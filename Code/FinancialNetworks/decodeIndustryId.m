function [ industry ] = decodeIndustryId( i )
    if i == 1
        industry = 'Oil and Gas';
    elseif i == 2
        industry = 'Basic Materials';
    elseif i == 3
        industry = 'Industrials';
    elseif i == 4
        industry = 'Consumer Goods';
    elseif i == 5
        industry = 'Health Care';
    elseif i == 6
        industry = 'Consumer Services';
    elseif i == 7
        industry = 'Telecommunications';
    elseif i == 8
        industry = 'Utilities';
    elseif i == 9
        industry = 'Financials';
    elseif i == 10
        industry = 'Technology';
    else
        industry =  '';
    end    
end