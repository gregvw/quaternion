from quaternion import Quaternion as Q
import sys

if __name__ == '__main__':

    q = Q(1,2,3,4)
    r = Q(0,-2,1,-4)
 
    print("\nTest quaternions")
    print("q       =  " + str(q))
    print("r       =  " + str(r))
    print("\nQuaternion operations and functions")
    print("q+r     =  " + str(q+r))
    print("q+r     =  " + str(q+r))
    print("q*r     =  " + str(q*r))
    print("-q      = " + str(-q))
    print("+q      =  " + str(+q))
    print("r/q     = " + str(r/q))
    print("|q|     =  " + str(abs(q)))
    print("conj(q) =  " + str(Q.conj(q)))
    print("exp(q)  =  " + str(Q.exp(q)))
    print("log(q)  =  " + str(Q.log(q)))
#   print("pow(q,2) =  " + str(Q.pow(q,2))) # Not working - returns zeros
    if sys.version_info[0]>=3: # These don't work in Python 2.7
        print("\nQuaternion operations with scalars")
        print("2+q     =  " + str(2+q))
        print("q-3     =  " + str(2+q))
        print("2*q     =  " + str(2*q))
        print("q/2     =  " + str(q/2))
