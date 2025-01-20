package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import jakarta.persistence.EntityExistsException;
import jakarta.persistence.EntityManager;
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
    private final JPAPropertyRepository propertyRepository;

    public CustomerServiceImpl(JPACustomerRepository customerRepository, JPAPropertyRepository propertyRepository) {
        this.customerRepository = customerRepository;
        this.propertyRepository = propertyRepository;
    }

    @Override
    public Customer getEntityById(String email) throws EntityNotFoundException {
        return customerRepository.findById(email).orElseThrow(() -> new EntityNotFoundException("Customer " + email + " not found."));
    }

    @Override
    public List<Customer> getEntitiesByIds(List<String> emails) {
        if(emails == null || emails.isEmpty()) return new ArrayList<>();
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
    public Customer createEntity(Customer customer) throws EntityExistsException, IllegalArgumentException {
        if(customerRepository.existsById(customer.getEmail())) {
           throw new EntityExistsException("Customer " + customer.getEmail() + " already exists.");
        }
        String email = customer.getFirstName() + "." + customer.getLastName() + "@mail.com";
        if(customer.getBirthDate().isBefore(LocalDate.now()) && customer.getEmail().equals(email)) {
            return customerRepository.save(customer);
        }
        throw new IllegalArgumentException("Invalid customer data input.");
    }

    @Override
    public Customer updateEntity(Customer customer) throws EntityNotFoundException {
        if(customerRepository.existsById(customer.getEmail())) {
            Customer oldCustomer = customerRepository.getReferenceById(customer.getEmail());
            Customer newCustomer = new Customer(oldCustomer.getEmail(), oldCustomer.getFirstName(),
                    oldCustomer.getLastName(), customer.getBirthDate(), oldCustomer.getSubleases());
            return customerRepository.save(newCustomer);
        }
        throw new EntityNotFoundException("Provided Customer " + customer.getEmail() + " not found.");
    }

    @Override
    public void deleteEntity(String email) throws EntityNotFoundException {
        if(customerRepository.existsById(email)) {
            Customer customer = customerRepository.getReferenceById(email);
            for (Property property: customer.getSubleases()) {
                propertyRepository.getReferenceById(property.getId()).setCustomer(null);
                propertyRepository.saveAndFlush(property);
            }
            customerRepository.getReferenceById(email).getSubleases().clear();
            customerRepository.saveAndFlush(customer);
            customerRepository.deleteById(email);
            return;
        }
        throw new EntityNotFoundException("Customer " +  email + "not found.");
    }

    @Override
    public void deleteCustomersBornAfter(LocalDate date) throws IllegalArgumentException {
        List<Customer> customersToDelete = customerRepository.getCustomersBornAfter(date);
        if(customersToDelete == null || customersToDelete.isEmpty()) return;
        for(Customer customer : customersToDelete) {
            for (Property property: customer.getSubleases()) {
                propertyRepository.getReferenceById(property.getId()).setCustomer(null);
                propertyRepository.saveAndFlush(property);
            }
            customerRepository.getReferenceById(customer.getEmail()).getSubleases().clear();
            customerRepository.saveAndFlush(customer);
        }
        customerRepository.deleteCustomersBornAfter(date);
    }
}
