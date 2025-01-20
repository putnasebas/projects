package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;

import java.time.LocalDate;
import java.util.List;

public interface CustomerService extends Service<Customer, String> {
    void deleteCustomersBornAfter(LocalDate date);
}
