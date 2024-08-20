use numpy::{PyArray1, PyArray2};
use pyo3::prelude::*;
use pyo3::types::PyAny;
use num_complex::Complex64;
use rayon::prelude::*;
use pyo3::Bound;

#[pyfunction]
fn rho_q(
    py_x: Bound<'_, PyAny>, 
    py_q: Bound<'_, PyAny>, 
    py_rho: Bound<'_, PyAny>
) -> PyResult<()> {
    // Convert Bound<'_, PyAny> to PyArray types
    let x = py_x.extract::<&PyArray2<f64>>()?;
    let q = py_q.extract::<&PyArray2<f64>>()?;
    let rho = py_rho.extract::<&PyArray1<Complex64>>()?;

    // Convert PyArray to slices
    let x = unsafe { x.as_slice().unwrap() };
    let q = unsafe { q.as_slice().unwrap() };
    let rho = unsafe { rho.as_slice_mut().unwrap() };

    let nx = x.len() / 3;
    let _nq = q.len() / 3;

    // Parallel computation
    rho.par_iter_mut().enumerate().for_each(|(i, rho_value)| {
        let mut result = Complex64::new(0.0, 0.0);

        for j in 0..nx {
            let alpha = x[j * 3] * q[i * 3]
                      + x[j * 3 + 1] * q[i * 3 + 1]
                      + x[j * 3 + 2] * q[i * 3 + 2];
            result += Complex64::new(0.0, alpha).exp();
        }

        *rho_value = result;
    });

    Ok(())
}

#[pymodule]
fn rho_q_rust(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(rho_q, m)?)?;
    Ok(())
}
