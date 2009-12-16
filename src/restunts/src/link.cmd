wlink @linkfile.
@copy restunts.exe s:\stunts\ /y

@rem /m = generate map file
@rem /n = ignore default libraries
@rem /Oa = minimum segment alignment (general error)
@rem /Or = minimum resource alignment (general error)
@rem /A=dd  Set segment alignment
@rem /f omit far/near optimization
@rem c:\tasm\bin\tlink /m /n /A=0 /f @linkfile
@rem s:\tools\dm\bin\link @linkfile
@rem qlink @linkfile
