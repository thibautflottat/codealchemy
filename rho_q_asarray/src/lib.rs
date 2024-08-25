use numpy::ndarray::ArrayView2;
use numpy::{PyArray2, PyReadonlyArray2};
use pyo3::prelude::*;
use num_complex::Complex64;

#[pyfunction]
fn rho_q_py<'py>(
    x: PyReadonlyArray2<'py, f64>,
    q: PyReadonlyArray2<'py, f64>,
    rho: &PyArray2<Complex64>,
) -> PyResult<()> {
    let x = x.as_array();
    let q = q.as_array();
    let mut rho = unsafe { rho.as_array_mut() };

    for (i, q_row) in q.outer_iter().enumerate() {
        for (j, x_row) in x.outer_iter().enumerate() {
            let alpha = x_row.dot(&q_row);
            rho[[i, j]] = Complex64::new(alpha.cos(), alpha.sin());
        }
    }

    Ok(())
}

#[pymodule]
fn rho_q_asarray(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(rho_q_py, m)?)?;
    Ok(())
}