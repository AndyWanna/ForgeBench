import pytest

from verification.fullmodel.references.arithmetic import set_storage_modes


@pytest.fixture(autouse=True)
def default_storage_modes():
    """Project() sets the process-global data_t storage modes from the saved typedef (RND/SAT for the <32,10> projects);
    reset them to the AP_TRN/AP_WRAP default so later tests see the default arithmetic."""
    set_storage_modes()
    yield
    set_storage_modes()
