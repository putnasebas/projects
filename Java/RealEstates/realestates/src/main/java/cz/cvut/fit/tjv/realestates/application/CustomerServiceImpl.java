package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;


@Service
//Transactional = if a method within transaction fails, it provides easier rollback
// -> saves data integrity
@Transactional
public class CustomerServiceImpl implements CustomerService {
    private final JPACustomerRepository customerRepository;

    public CustomerServiceImpl(JPACustomerRepository customerRepository) {
        this.customerRepository = customerRepository;
    }

    @Override
    public Customer getEntityById(String email) throws EntityNotFoundException {
        return customerRepository.findById(email).orElseThrow(() -> new EntityNotFoundException("Customer " + email + " not found."));
    }

    @Override
    public List<Customer> getEntitiesByIds(List<String> emails) {
        List<Customer> customers = new ArrayList<>();
        for (String email : emails) {
            if(!customerRepository.existsById(email)) {
                throw new EntityNotFoundException("Customer " + email + " not found.");
            }
            customers.add(getEntityById(email));
        }
        return customers;
    }

    @Override
    public List<Customer> getAllEntities() {
        return customerRepository.findAll();
    }

    @Override
    public Customer createEntity(Customer customer) throws IllegalArgumentException {
        String email = customer.getFirstName() + "." + customer.getLastName() + "@mail.com";
        if(customer.getBirthDate().isBefore(LocalDate.now()) && customer.getEmail().equals(email)) {
            return customerRepository.save(customer);
        }
        throw new IllegalArgumentException("Invalid customer data input.");
    }

    @Override
    public Customer updateEntity(Customer customer) throws EntityNotFoundException {
        if(customerRepository.existsById(customer.getEmail())) {
            return customerRepository.save(customer);
        }
        throw new EntityNotFoundException("Provided Customer " + customer.getEmail() + " not found.");
    }

    @Override
    public void deleteEntity(String email) throws EntityNotFoundException {
        if(customerRepository.existsById(email)) {
            customerRepository.deleteById(email);
            return;
        }
        throw new EntityNotFoundException("Customer " +  email + "not found.");
    }


}
