function testCubic()

    expectedRoots = [
%         1, 2, 3;
%         1, 10, 10;
%         1, 100, 1000;
%         -2, 2, 5;
%         1j, -1j, 6;
%         2 + 1j, 2 - 1j, 43;
%         1, -2,  1;
%         1,0,5;
%         0,1,4;
%          1 1 1;

%     1e-3 1e-4 1e-5; % small numbers
%     1e-20 1e-18 1e-15; % very small numbers
%     1e-5 1e-8 1e-10; % large and small numbers
      10, 10, 10
%     1e35 1e15 1e-20;
%     1e50 1e15 1e-18;
%     5e-28 3.3e-30 8.7e-33;
%     1e100 5e80 1e20;
    % test quadratic equations
    % 0 will be trimmed before running test
%     0 1e20 1e40
%     0 5e-15 1e-13
%     0 15 13
%     0 0 5
%     0 0 -5
%     0 0 1.3e10
%     0 0 -5.6e10
%     0 0 5.6e-10
    % imaginary numbers
%     1+i 1-i 1;
%     4.41 5-2i 5+2i;
%     61.32 -11+98i -11-98i;
%         1j,1j,0;
%         0,1j,2;
%         1j, 1 ,1; 
    ];
    tolerance = 1e-6;

    numPolynomials = size(expectedRoots, 1);
    failures = 0;
    for index = 1:numPolynomials
        r1 = expectedRoots(index, 1);
        r2 = expectedRoots(index, 2);
        r3 = expectedRoots(index, 3);
        % NOTE: We assume r3 is the largest expected.
        [a, b, c, d] = rootsToCoeffs(r1, r2, r3);
        [largestRoot] = cubic(a, b, c, d);
        % Compare the actual and expected.
        maxRelErr = compareRoots(r3, largestRoot);
        if maxRelErr > tolerance
            failures = failures + 1;
            disp('Failed to find roots to precision');
            disp('Expected:');
            disp(r3);
            disp('Actually computed:');
            disp(largestRoot);
        end
    end
    if failures == 0
        disp('Success');
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Helpers for root comparison %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [a, b, c, d] = rootsToCoeffs(r1, r2, r3)
    a = 1;
    b = -(r1 + r2 + r3);
    c = r1 * r2 + r2 * r3 + r3 * r1;
    d = -(r1 * r2 * r3);
end

function maxRelErr = compareRoots(expected, actual)
    maxRelErr = -Inf;
    if expected == 0
        if actual ~= 0
            % If we wanted 0 but didn't have it, infinite error.
            maxRelErr = Inf;
        end
    else
        relErr = abs(expected - actual) / abs(expected);
        % Make sure error of 0.0 isn't a bad thing.
        if relErr ~= 0
            maxRelErr = relErr;
        end
    end
end

