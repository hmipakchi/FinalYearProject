function [ dateIndex ] = getDateIndexFromDate( datesVector, date )
    noDates = size(datesVector{1,1});
    dateIndex = -1;
    for i=1:noDates
        if (i == 1)
            if datenum(datesVector{1,1}{i}) > datenum(date)
                dateIndex = i;
            end
        elseif (i == noDates)
        	if datenum(datesVector{1,1}{i}) < datenum(date)
                dateIndex = i;
            end
        else
            if (datenum(datesVector{1,1}{i-1}) < datenum(date)) && (datenum(datesVector{1,1}{i+1}) > datenum(date))
                dateIndex = i;
            end
        end
    end
end