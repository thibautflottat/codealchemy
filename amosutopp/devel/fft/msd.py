import numpy as np

def autocorrFFT(x):
    N = len(x)
    F = np.fft.fft(x, n=2 * N)  # 2*N because of zero-padding
    PSD = F * F.conjugate()
    res = np.fft.ifft(PSD)
    res = (res[:N]).real  # now we have the autocorrelation in convention B
    n = N * np.ones(N) - np.arange(0, N)  # divide res(m) by (N-m)
    return res / n  # this is the autocorrelation in convention A

def mixed_corrFFT(x1, x2):
    """Cross-correlation between x1 and x2 using FFT"""
    N = len(x1)
    F1 = np.fft.fft(x1, n=2 * N)  # 2*N because of zero-padding
    F2 = np.fft.fft(x2, n=2 * N)  # 2*N because of zero-padding
    PSD = F1 * F2.conjugate()
    res = np.fft.ifft(PSD)
    res = (res[:N]).real  # now we have the cross-correlation
    n = N * np.ones(N) - np.arange(0, N)  # divide res(m) by (N-m)
    return res / n  # this is the cross-correlation

def fourth_order_fft(r):
    N = len(r)
    
    # Fourth power displacement moments
    D4 = np.power(r, 4).sum(axis=1)
    D4 = np.append(D4, 0)
    
    # Cross-terms for r(t+tau)^2 r(t)^2
    S2_2 = sum([autocorrFFT(np.square(r[:, i])) for i in range(r.shape[1])])
    
    # Third order cross-terms: <r(t+tau)^3 * r(t)>
    S3_1 = sum([mixed_corrFFT(np.power(r[:, i], 3), r[:, i]) for i in range(r.shape[1])])
    
    # Third order cross-terms: <r(t+tau) * r(t)^3>
    S1_3 = sum([mixed_corrFFT(r[:, i], np.power(r[:, i], 3)) for i in range(r.shape[1])])
    
    # Fourth moment calculation (includes cubic terms)
    Q4 = 2 * D4.sum()
    S1_4 = np.zeros(N)
    for m in range(N):
        Q4 = Q4 - D4[m - 1] - D4[N - m]
        S1_4[m] = Q4 / (N - m)
    
    # Return the full fourth moment with cubic and cross terms
    return S1_4 - 4 * S3_1 + 6 * S2_2 - 4 * S1_3

def ngp_fft(r):
    # First, compute the MSD using FFT
    msd = msd_fft(r)
    
    # Now, compute the fourth-order moment using FFT
    fourth_moment = fourth_order_fft(r)
    
    # Compute the NGP based on the formula (3/5) * (fourth_moment / msd^2) - 1
    ngp = np.zeros_like(msd)
    
    # Ensure no division by zero for msd
    valid_indices = (msd > 0) & (fourth_moment > 0)  # Only compute where MSD and fourth moment are valid
    ngp[valid_indices] = (3 / 5) * (fourth_moment[valid_indices] / (msd[valid_indices] ** 2)) - 1
    
    # Explicitly set NGP to zero at tau = 0 (since it's undefined)
    ngp[0] = 0
    
    return ngp

def msd_fft(r):
    N = len(r)
    D = np.square(r).sum(axis=1)
    D = np.append(D, 0)
    
    # Autocorrelation for each coordinate
    S2 = sum([autocorrFFT(r[:, i]) for i in range(r.shape[1])])
    
    Q = 2 * D.sum()
    S1 = np.zeros(N)
    
    for m in range(N):
        Q = Q - D[m - 1] - D[N - m]
        S1[m] = Q / (N - m)
    
    return S1 - 2 * S2

# Test with random data
r = np.random.rand(10, 3) * 10

# Direct calculation for comparison
msd_direct = np.zeros(len(r))
ngp_direct = np.zeros(len(r))
lagtimes = np.arange(1, len(r))

for lag in lagtimes:
    disp = r[:-lag, :] - r[lag:, :]
    sqdist = np.square(disp).sum(axis=-1)
    msd_direct[lag] = np.mean(sqdist, axis=0)
    ngp_direct[lag] = 3.0 / 5.0 * np.mean(np.square(sqdist), axis=0) / np.square(msd_direct[lag]) - 1.0

# FFT-based MSD and NGP
msd_fft_result = msd_fft(r)
ngp_fft_result = ngp_fft(r)

# Compare results
print("Direct MSD: ", msd_direct)
print("FFT-based MSD: ", msd_fft_result)
print("Direct NGP: ", ngp_direct)
print("FFT-based NGP: ", ngp_fft_result)